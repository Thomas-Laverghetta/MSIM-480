import enum
import random
from PIL import Image

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
    
    # pops the last node in the linked list
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
    def __init__(self, cond=None):
        self.R = 0
        self.G = 0
        self.B = 0
        if cond == None:
            self.R = random.randint(0,255)
            self.G = random.randint(0,255)
            self.B = random.randint(0,255)

# Container for X,Y points
class Vector2D:
    def __init__(self, imageSize):
        self.x = random.randint(0, imageSize[0])
        self.y = random.randint(0, imageSize[1])

# container for traingle vector
class Triangle:
    def __init__(self, imageSize):
        self.p1 = Vector2D(imageSize)
        self.p2 = Vector2D(imageSize)
        self.p3 = Vector2D(imageSize)
        self.rgb = RGB()

class GeneticImage:
    class TriangleSet:
        def __init__(self, resolution, image):
            self.triangles = []
            for _ in range(resolution):
                self.triangles.append(Triangle(image.size))
            
            self.fitness = self.IndividualFitness(image)
        
        def IndividualFitness(self, image):
            global ImageSize
            colorMatrix = []

            # initialize color matrix
            for _ in range(ImageSize[0]):
                colorMatrix.append([RGB(True)] * ImageSize[1])
            
            for tri in self.triangles:
                maxY = self.MaxY(tri.p1, tri.p2, tri.p3)

                # minX is the left most and lowest Y, and minX2 is right most and lowest Y
                minX, minX2 = self.MinXY(tri.p1,tri.p2,tri.p3,maxY)
                
                # bottom line
                yb = lambda xb: ceil((minX.y - minX2.y)/(minX.x - minX2.x) * (xb - minX.x) + minX.y)

                # y1 is left line and y2 is right line 
                y1 = lambda x1: ceil((minX.y - maxY.y)/(minX.x - maxY.x) * (x1 - minX.x) + minX.y)
                y2 = lambda x2: ceil((minX2.y - maxY.y)/(minX2.x - maxY.x) * (x2 - minX2.x) + minX2.y)
                
                for x in range(minX.x * (minX.x<maxY.x) + maxY.x * (maxY.x < minX.x), minX2.x * (minX2.x<maxY.x) + maxY.x * (maxY.x < minX2.x)):
                    
                # TO DO:
                    # find area between lines (minX,minX2) and (minX,maxY) and (maxY,minX2)



            del colorMatrix
            return 0
        
        def MinXY(self, p1,p2,p3,maxY):
            if p1 == maxY:
                if p2.x < p3.x:
                    return p2,p3
                else:
                    return p3,p2
            elif p2 == maxY:
                if p1.x < p3.x:
                    return p1,p3
                else:
                    return p3,p1
            else:
                if p1.x < p2.x:
                    return p1,p2
                else:
                    return p2,p1

        def MaxY(self, p1,p2,p3):
            tmp = 0
            if p1.y < p2.y:
                tmp = p2
            else:
                tmp = p1
            
            if p3.y > tmp.y:
                return p3
            else:
                return tmp
            
    def __init__(self, initPopulation, geneLength, image):
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
                self.Mutation(geneLength)
            
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
