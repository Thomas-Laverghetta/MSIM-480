import enum
import random
import numpy as np
from functools import reduce
from copy import deepcopy 
from PIL import Image, ImageDraw, ImageChops
import time
import math
import operator


# Sorted Linked List data structure for triangle set
class ListNode:
    def __init__(self, dataval=None):
        self.node = dataval
        self.nextval = None
        self.prevval = None

class SortedLinkedList:
    def __init__(self):
        self.headval = None
        self.tailval = None
        self.counter = 0
        self.TotalFitness = 0.0

    # sorted insert
    def AddNode(self, node):
        new_listNode = ListNode(node)
        if self.headval == None and self.tailval == None:
            self.headval = self.tailval = new_listNode
        elif self.headval.node.fitness >= new_listNode.node.fitness:
            new_listNode.nextval = self.headval
            self.headval.prevval = new_listNode
            self.headval = new_listNode
        else:
            curr = self.headval
            while curr.nextval != None and curr.nextval.node.fitness < new_listNode.node.fitness:
                curr = curr.nextval

            new_listNode.nextval = curr.nextval
            if curr.nextval != None:
                curr.nextval.prevval = new_listNode

            new_listNode.prevval = curr
            curr.nextval = new_listNode

            if curr == self.tailval:
                self.tailval = new_listNode

        self.counter = self.counter + 1
        self.TotalFitness += node.fitness
    
    # returns head of linked list
    def GetHead(self):
        return self.headval

    def GetRandParents(self):
        p1_r = random.uniform(0, 1)
        p2_r = random.uniform(0, 1)
        parent1 = None
        parent2 = None

        curr = self.headval

        # The sum of all weights from 1 to counter
        # NormSum = self.counter * (self.counter + 1.0) / 2.0

        sum = 0
        # i = self.counter
        while(parent1 == None or parent2 == None):
            sum += curr.node.fitness
            pmf = (sum / self.TotalFitness)
            if parent1 == None and p1_r <= pmf:
                parent1 = curr.node # deepcopy(curr.node)
            
            if parent2 == None and p2_r <= pmf:
                parent2 = curr.node # deepcopy(curr.node)

            curr = curr.nextval

        return parent1, parent2

    # Get head of list (smallest f)
    def Pop(self):
        curr = self.headval
        self.headval = self.headval.nextval
        self.headval.prevval = None

        node = curr.node

        del curr
        curr = None
        self.counter = self.counter - 1
        self.TotalFitness -= node.fitness
        return node
    
    # pops the last node in the linked list
    def PopLast(self):
        curr = self.tailval
        self.tailval = self.tailval.prevval
        self.tailval.nextval = None

        node = curr.node

        del curr
        curr = None
        self.counter = self.counter - 1
        self.TotalFitness -= node.fitness
        return node



# container for traingle vector
class Triangle:
    # Container for RBG values
    class RGBA:
        def __init__(self):
            self.R = random.randint(0,255)
            self.G = random.randint(0,255)
            self.B = random.randint(0,255)
            self.A = 128
        
        def RGBA_Mutate(self):
            self.R = random.randint(0,255)
            self.G = random.randint(0,255)
            self.B = random.randint(0,255)

    # Container for X,Y points
    class Vector2D:
        def __init__(self):
            self.x = random.randint(0, src_img_w)
            self.y = random.randint(0, src_img_h)
        
        def PointMutate(self):
            self.x = random.randint(0, src_img_w)
            self.y = random.randint(0, src_img_h)

    def __init__(self):
        self.p1 = self.Vector2D()
        self.p2 = self.Vector2D()
        self.p3 = self.Vector2D()
        self.rgba = self.RGBA()
    
    def TriMutate(self):
        self.p1.PointMutate()
        self.p2.PointMutate()
        self.p3.PointMutate()
        self.rgba.RGBA_Mutate()

class GeneticImage:
    class TriangleSet:
        def __init__(self, resolution):
            self.triangles = []
            for _ in range(resolution):
                self.triangles.append(Triangle())
            
            self.fitness = 0

            self.img = None

            self.Fitness()
        
        def generate_image(self):
            img = Image.new('RGBA', (src_img_w, src_img_h))
            draw = ImageDraw.Draw(img, 'RGBA')
            for g in self.triangles:
                draw.polygon([(g.p1.x, g.p1.y), (g.p2.x, g.p2.y), (g.p3.x, g.p3.y)], (g.rgba.R,g.rgba.B,g.rgba.G,g.rgba.A))
            
            del draw
            self.img = img # = np.asarray(img)

        def Fitness(self):
            self.generate_image()
            # Image.fromarray(self.img, 'RGBA')
            h = ImageChops.difference(src_img, self.img).histogram()
            self.fitness = math.sqrt(reduce(operator.add,
                            map(lambda h, i: h*(i**2),
                             h, list(range(256))*3)) /
                            (float(src_img.size[0]) * src_img.size[1]))
        
        def save_image(self, file_name):
            out_pic = Image.fromarray(np.asarray(self.img), 'RGBA')
            out_pic.save(file_name,'PNG')
            
    def __init__(self, initPopulation, geneLength):
        self.population = SortedLinkedList()            # Generating linked list
        self.child = self.TriangleSet(geneLength)
        self.parent1 = None                             # Variable for the fittest 
        self.parent2 = None                       # variable for the second fittest
        self.generationCount = 0                        # The number of generation
        self.initPopulation = initPopulation
        for _ in range(initPopulation):                 # initializing all the triangle sets
            self.population.AddNode(self.TriangleSet(geneLength))

        # generate first best image    
        self.population.headval.node.save_image('Generation ' + str(self.generationCount) + '.png')

        # the generation control loop
        while self.population.headval.node.fitness > 5 and self.generationCount < 100000:
            self.generationCount = self.generationCount + 1
            
            children = []
            for _ in range(5):
                self.Selection()
                self.CrossOver(geneLength)

                # 20% chance of mutation
                #if random.uniform(0,1) < 0.05:
                self.Mutation(geneLength)
                self.child.Fitness()
                children.append(deepcopy(self.child))
            
            # remove the least fittest
            for i in range(5):
                self.population.PopLast()
                self.population.AddNode(children[i])
            
            if self.generationCount % 5000 == 0:
                print(self.population.headval.node.fitness)
                self.population.headval.node.save_image('Generation ' + str(self.generationCount) + '.png')
            elif self.generationCount % 500 == 0:
                print(self.population.headval.node.fitness)

        self.population.headval.node.save_image('Generation ' + str(self.generationCount) + '.png')

    # select two random parents from population     
    def Selection(self):
        self.parent1, self.parent2 = self.population.GetRandParents()

    # mixes genes between fittest and secondFittest
    def CrossOver(self, geneLength):
        for i in range(geneLength):
            if random.uniform(0,1) < 0.5:
                self.child.triangles[i] = deepcopy(self.parent1.triangles[i])
            else:
                self.child.triangles[i] = deepcopy(self.parent2.triangles[i])
            

    def Mutation(self, geneLength):
        # # calculating mutation point
        # mutationPoint = random.randint(0, geneLength - 1)

        for i in range(geneLength):
            if random.uniform(0, 1) < 0.05:
                self.child.triangles[i].TriMutate()

if __name__ == "__main__":
    global src_img
    global src_img_w
    global src_img_h

    # set image and get size of image
    src_img = Image.open('Facebook_Logo.png')
    src_img_w, src_img_h = src_img.size

    # convert image to RGBA if not already
    if src_img.format != 'RGBA':
        src_img = src_img.convert('RGBA')
    
    GeneticImage(25,100)
    