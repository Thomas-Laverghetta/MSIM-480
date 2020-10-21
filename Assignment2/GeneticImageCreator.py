import enum
import random
import numpy as np
from functools import reduce
from copy import deepcopy 
from PIL import Image, ImageDraw, ImageChops
import time
import math
import operator
import os
import sys



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
        self.TotalInverseFitness = 0.0

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
        self.TotalInverseFitness += 1.0/node.fitness
    
    # returns head of linked list
    def GetHead(self):
        return self.headval

    def GetRandParents(self):
        p1_r = random.uniform(0, 1)
        p2_r = random.uniform(0, 1)
        parent1 = None
        parent2 = None

        curr = self.headval

        probSum = 0.0
        while(parent1 == None or parent2 == None):
            probSum += 1.0/curr.node.fitness
            pmf = (probSum / self.TotalInverseFitness)
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
        self.TotalInverseFitness -= 1.0/node.fitness
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
        self.TotalInverseFitness -= 1.0/node.fitness
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
            # Fitness function was taken from https://github.com/DING-PENG/image-approx
            self.generate_image()
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
        self.population.headval.node.save_image(BATCH_PATH+'/GeneticGeneration ' + str(self.generationCount) + '.png')

        # the generation control loop
        while self.population.headval.node.fitness > 5 and self.generationCount < 100000:
            self.generationCount = self.generationCount + 1
            
            children = []
            for _ in range(5):
                self.Selection()
                self.CrossOver(geneLength)
                self.Mutation(geneLength)
                self.child.Fitness()
                children.append(deepcopy(self.child))
            
            # remove the least fittest
            for i in range(5):
                self.population.PopLast()
                self.population.AddNode(children[i])
            
            if self.generationCount in [100,1000,10000,20000,30000,50000,10000]:
                print(self.population.headval.node.fitness)
                self.population.headval.node.save_image(BATCH_PATH+'/GeneticGeneration ' + str(self.generationCount) + '.png')
            elif self.generationCount % 500 == 0:
                print(self.population.headval.node.fitness)

        self.population.headval.node.save_image(BATCH_PATH+'/GeneticGeneration ' + str(self.generationCount) + '.png')

    # select two random parents from population     
    def Selection(self):
        # selecting two weighted random parents from population
        self.parent1, self.parent2 = self.population.GetRandParents()

    # mixes genes between fittest and secondFittest
    def CrossOver(self, geneLength):
        # the crossover of parents
        for i in range(geneLength):
            if random.uniform(0,1) < CROSS_OVER_PROB:
                self.child.triangles[i] = deepcopy(self.parent1.triangles[i])
            else:
                self.child.triangles[i] = deepcopy(self.parent2.triangles[i])
            

    def Mutation(self, geneLength):
        # Mutation of individual genes
        for i in range(geneLength):
            if random.uniform(0, 1) < MUTATION_PROB:
                self.child.triangles[i].TriMutate()

if __name__ == "__main__":
    global src_img
    global src_img_w
    global src_img_h
    global MUTATION_PROB
    MUTATION_PROB = float(sys.argv[5])
    global CROSS_OVER_PROB
    CROSS_OVER_PROB = float(sys.argv[4])
    PATH_SOURCE_IMAGE = "Images/" + str(sys.argv[1])

    POPULATION_SIZE = int(sys.argv[2])
    NUM_TRIANGLES_PER_GENE = int(sys.argv[3])


    # Creates batch folder to save all boids from multiple runs
    global BATCH_PATH
    BATCH_PATH = PATH_SOURCE_IMAGE + "_pop" + str(POPULATION_SIZE) + "_genelegnth" + str(NUM_TRIANGLES_PER_GENE) + "_mut" + str(MUTATION_PROB) + "_cross" + str(CROSS_OVER_PROB)
    try:
        os.makedirs(BATCH_PATH)
    except OSError:
        print ("Creation of the directory %s failed" % BATCH_PATH)
    else:
        print ("Successfully created the directory %s" % BATCH_PATH)

    
    # set image and get size of image
    src_img = Image.open(PATH_SOURCE_IMAGE + '.png')
    src_img_w, src_img_h = src_img.size

    # convert image to RGBA if not already
    if src_img.format != 'RGBA':
        src_img = src_img.convert('RGBA')
    
    GeneticImage(POPULATION_SIZE,NUM_TRIANGLES_PER_GENE)
    