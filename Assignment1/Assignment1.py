from copy import deepcopy 
from collections import deque
import threading
import time
import tracemalloc
import os
import sys
import csv

cMax = 0
rMax = 0
nodesSearched = 0
class Node:
    state = []
    moveSet = []
    f = -1

# Manhattan Algorithm
def Heuristic(state):
    sum = 0
    row_center = (len(state) -1)/2.0
    col_center = (len(state[0]) - 1)/2.0
    for r in range(len(state)):
        for c in range(len(state[0])):
            sum = sum + (state[r][c] != 0) * (abs(r - row_center / 2) + abs(c - col_center / 2))
    return sum

# Goal determines whether given state is solution
def Goal(state):
    sum = 0
    for r in range(len(state)):
        for c in range(len(state[0])):
            sum = sum + (state[r][c] != -1)*state[r][c] # -1s are NULL elements
            if sum > 1:
                return False
    return True # TRUE if final state

# Successor determines naighboring states
def Successor(stateNode):
    neighboringStates = []
    for r in range(rMax):
        for c in range(cMax):
            if (stateNode.state[r][c] != 0):
                # Checking Below
                if ((r+2) < rMax and stateNode.state[r+2][c] == 0 and stateNode.state[r+1][c] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r+2][c] = 1
                    newNode.state[r+1][c] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet) 
                    newNode.moveSet.append([r,c,'S'])
                    neighboringStates.append(newNode)

                # Checking Above
                if ((r-2) >= 0 and stateNode.state[r-2][c] == 0 and stateNode.state[r-1][c] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r-2][c] = 1
                    newNode.state[r-1][c] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet)
                    newNode.moveSet.append([r,c,'N'])
                    neighboringStates.append(newNode)
                
                # Checking to the right
                if ((c+2) < cMax and stateNode.state[r][c+2] == 0 and stateNode.state[r][c+1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r][c+2] = 1
                    newNode.state[r][c+1] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet) 
                    newNode.moveSet.append([r,c,'E'])
                    neighboringStates.append(newNode)

                # checking to the left
                if ((c-2) >= 0 and stateNode.state[r][c-2] == 0 and stateNode.state[r][c-1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r][c-2] = 1
                    newNode.state[r][c-1] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet) 
                    newNode.moveSet.append([r,c,'W'])
                    neighboringStates.append(newNode)
    
    return neighboringStates

def StateToTuple(state):
    # convert to tuple
    temp = []
    for r in range(rMax):
        for c in range(cMax):
            temp.append(state[r][c])
    
    t = tuple(temp)
    temp.clear()
    return t

# input is a node
def Breadth_First_Search(InitNode):
    global nodesSearched
    visited = set() # array of visited and visiting states
    maxMove = 0

    # queue holding states to expand
    queue = deque()
    queue.append(InitNode)
    while len(queue) > 0:
        nodesSearched = nodesSearched + 1
        c = queue.popleft()      # Get next element of queue
        if Goal(c.state):        # if current state is goal
            return c
        else:
            # Get successor states from current state (c)
            neighbors = Successor(c)
            
            # if index is larger than maxMove, update move counter (went to next layer down of tree) 
            if (len(c.moveSet) - 1) == maxMove:
                visited.clear()
                maxMove = maxMove + 1

            # for each neighbor determine if state has not been searched
            for n in neighbors:
                # convert to tuple
                t = StateToTuple(n.state)

                # if no other states were found that matched this state (n)
                if t not in visited:   
                    visited.add(t)
                    queue.append(n) # add to queue
    return False
    
# input is a node
Depth_First_Search_VisitedList = set()
def Depth_First_Search(InitNode):
    global nodesSearched
    nodesSearched = nodesSearched + 1
    if Goal(InitNode.state):
        InitNode.f = 1
        return InitNode 
    else:
        neighbors = Successor(InitNode)
        for n in neighbors:
            # convert to tuple
            temp = []
            for r in range(rMax):
                for c in range(cMax):
                    temp.append(n.state[r][c])
            
            t = tuple(temp)
            temp.clear()

            if t not in Depth_First_Search_VisitedList:
                Depth_First_Search_VisitedList.add(t)
                Result = Depth_First_Search(n)

                # defualt is -1 and only changes 
                if Result.f == 1:
                    return Result

    return InitNode

# Sorted Linked List data structure for GBS and A* 
class ListNode:
    def __init__(self, dataval=None):
        self.node = dataval
        self.nextval = None

class SortedLinkedList:
    def __init__(self):
        self.headval = None
        self.counter = 0

    # sorted insert
    def AddNode(self, node):
        new_listNode = ListNode(node)
        if self.headval == None:
            new_listNode.nextval = self.headval
            self.headval = new_listNode
        elif self.headval.node.f >= new_listNode.node.f:
            new_listNode.nextval = self.headval
            self.headval = new_listNode
        else:
            curr = self.headval
            while curr.nextval != None and curr.nextval.node.f < new_listNode.node.f:
                curr = curr.nextval
            new_listNode.nextval = curr.nextval
            curr.nextval = new_listNode

        self.counter = self.counter + 1
    
    # Get head of list (smallest f)
    def Pop(self):
        curr = self.headval
        self.headval = self.headval.nextval

        node = curr.node

        del curr
        curr = None
        self.counter = self.counter - 1

        return node

# input is Node
def Greedy_Best_Search(InitNode):
    global nodesSearched

    if Goal(InitNode.state):
        return InitNode
    
    Open = SortedLinkedList()
    Closed = set()

    Open.AddNode(InitNode)
    while Open.counter > 0:
        nodesSearched = nodesSearched + 1
        # Pop the top
        N = Open.Pop()

        ne = Successor(N)
        for n in ne:
            t = StateToTuple(n.state)
            if t not in Closed:
                if Goal(n.state):
                    return n
                Closed.add(t)
                n.f = Heuristic(n.state)
                Open.AddNode(n)
    
    return False


def A_Star_Search(InitNode):
    global nodesSearched
    if Goal(InitNode.state):
        return InitNode

    Closed = set() # array of visited and visiting states
    Open = SortedLinkedList()
    Open.AddNode(InitNode)

    while Open.counter > 0:
        nodesSearched = nodesSearched + 1
        q = Open.Pop()
        neighbors = Successor(q)

        for n in neighbors:
            # convert to tuple
            t = StateToTuple(n.state)

            if t not in Closed:
                if Goal(n.state):
                    return n
                n.f = Heuristic(n.state) + len(n.moveSet)
                Closed.add(t)
                Open.AddNode(n)
    return False
    # end of WHILE

# prints the solution path to console                   
def PathPrint(initState, moveSet):
    print('Move',0)
    for j in initState:
        print(j)

    print('')

    count = 0
    for i in moveSet:
        count = count + 1
        r = i[0]
        c = i[1]
        if i[2] == 'S':
            initState[r+2][c] = 1
            initState[r+1][c] = initState[r][c] = 0
        elif i[2] == 'N':
            initState[r-2][c] = 1
            initState[r-1][c] = initState[r][c] = 0
        elif i[2] == 'E':
            initState[r][c+2] = 1
            initState[r][c+1] = initState[r][c] = 0
        else:
            initState[r][c-2] = 1
            initState[r][c-1] = initState[r][c] = 0

        print('Move',count)
        for j in initState:
            print(j)
        
        print('')


if __name__ == "__main__":
    Algorithms = ["Astar", "BFS", "DFS", "GBS"]
    SelectedAlgorithm = ""
    InitNode = Node()
    
    # reading in input file
    with open('input.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            if str(row[0]) in Algorithms:
                SelectedAlgorithm = str(row[0])
                continue
            temp = []
            for col in row:
                temp.append(int(col))
            InitNode.state.append(temp)


    # # letting global size variables
    cMax = len(InitNode.state[0])
    rMax = len(InitNode.state)

    if cMax != rMax:
        print("ERROR: Invalid Input sizes\n\a")
        os._exit(0)

    # What algorithm is selected
    if SelectedAlgorithm == "DFS":
        path = Depth_First_Search(InitNode)
    elif SelectedAlgorithm == "BFS":
        path = Breadth_First_Search(InitNode)
    elif SelectedAlgorithm == "GBS":
        path = Greedy_Best_Search(InitNode)
    elif SelectedAlgorithm == "Astar":
        path = A_Star_Search(InitNode)
    else:
        print("ERROR: Invalid Search Algorithm inputted\n\a")
        os._exit(0)
    
    # if a solution was found
    if path != False and Goal(path.state):
        PathPrint(InitNode.state, path.moveSet)
    else:
        print("NO SOLUTION FOUND\n")

    print("Solution Path (r,c,direction):", path.moveSet)    
    