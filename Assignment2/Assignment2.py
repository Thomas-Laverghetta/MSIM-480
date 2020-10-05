import enum
import random
from PIL import Image

ImageSize = tuple()

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

    # sorted insert
    def AddNode(self, node):
        new_listNode = ListNode(node)
        if self.headval == None and self.tailval == None:
            new_listNode.nextval = new_listNode.prevval = self.headval
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
            new_listNode.prevval = curr
            curr.nextval = new_listNode

            if curr == self.tailval:
                self.tailval = new_listNode

        self.counter = self.counter + 1
    
    # returns head of linked list
    def GetHead(self):
        return self.headval

    # Get head of list (smallest f)
    def Pop(self):
        curr = self.headval
        self.headval = self.headval.nextval
        self.headval.prevval = None

        node = curr.node

        del curr
        curr = None
        self.counter = self.counter - 1

        return node
    
    def PopLast(self):
        curr = self.tailval
        self.tailval = self.tailval.prevval
        self.tailval.nextval = None

        node = curr.node

        del curr
        curr = None
        self.counter = self.counter - 1

        return node

# Container for RBG values
class RGB:
    R = random.randint(0,255)
    G = random.randint(0,255)
    B = random.randint(0,255)

# Container for X,Y points
class Vector2D:
    x = random.randint(0, ImageSize[0])
    y = random.randint(0, ImageSize[1])

# container for traingle vector
class Triangle:
    p1 = Vector2D()
    p2 = Vector2D()
    p3 = Vector2D()
    rgb = RGB()

class GeneticImage:
    class TriangleSet:
        def __init__(self, resolution, image):
            self.triangles = []
            for _ in range(resolution):
                self.triangles.append(Triangle())
            
            self.fitness = self.IndividualFitness(image)
        
        def IndividualFitness(self, image):
            return 0
    
    def __init__(self, initPopulation, geneLength, image):
        self.colorMatrix = []
        self.population = SortedLinkedList()
        self.fittest = None
        self.secondFittest = None
        self.generationCount = 0
        for _ in range():
            self.population.AddNode(self.TriangleSet(geneLength, image))
        
        while self.population.headval.fitness < 5:
            self.generationCount = self.generationCount + 1
            
            self.Selection()
            self.CrossOver(geneLength)
            if random.randint(0,7) < 5:
                self.Mutation()
            
            # recalculating after crossover and mutation
            self.fittest.IndividualFitness(image)
            self.secondFittest.IndividualFitness(image)

            # remove the least fittest
            self.population.PopLast()

            # add most fittest after crossover and mutation
            if self.fittest.fitness > self.secondFittest.fitness:
                self.population.AddNode(self.fittest)            
            else:
                self.population.AddNode(self.secondFittest)

    # select fittest and second fittest     
    def Selection(self):
        head = self.population.GetHead()            # get head of list (points to the fittest)
        self.fittest = head.node                    # get fittest node
        self.secondFittest = head.nextval.node      # get second fittest node (sorted linked list, so second)

    # mixes genes between fittest and secondFittest
    def CrossOver(self, geneLength):
        crossOverPoint = random.randint(0,geneLength - 1)
        for i in range(crossOverPoint):
            temp = self.fittest.triangles[i]
            self.fittest.triangles[i] = self.secondFittest.triangles[i]
            self.secondFittest.triangles[i] = temp

    def Mutation(self, geneLength):
        # calculating mutation point
        mutationPoint = random.randint(0, geneLength - 1)

        # flipping color mutation
        self.fittest.triangles[mutationPoint].rgb.B = 255 - self.fittest.triangles[mutationPoint].rgb.B
        self.fittest.triangles[mutationPoint].rgb.G = 255 - self.fittest.triangles[mutationPoint].rgb.G  
        self.fittest.triangles[mutationPoint].rgb.R = 255 - self.fittest.triangles[mutationPoint].rgb.R  

        # calculating mutation point for second fittest
        mutationPoint = random.randint(0, geneLength - 1)

        # flippng color mutation
        self.secondFittest.triangles[mutationPoint].rgb.B = 255 - self.secondFittest.triangles[mutationPoint].rgb.B
        self.secondFittest.triangles[mutationPoint].rgb.G = 255 - self.secondFittest.triangles[mutationPoint].rgb.G  
        self.secondFittest.triangles[mutationPoint].rgb.R = 255 - self.secondFittest.triangles[mutationPoint].rgb.R 



if __name__ == "__main__":
    im = Image.open('plane.jpg') # Can be many different formats.
    pix = im.load()         # Load Image
    ImageSize = im.size     # Get the width and hight of the image for iterating over
