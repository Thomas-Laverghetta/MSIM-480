from copy import deepcopy 
from collections import deque
import threading
import time
import tracemalloc
import os
import sys

cMax = 0
rMax = 0
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
                    newNode.moveSet.append([r,c,'D'])
                    neighboringStates.append(newNode)

                # Checking Above
                if ((r-2) >= 0 and stateNode.state[r-2][c] == 0 and stateNode.state[r-1][c] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r-2][c] = 1
                    newNode.state[r-1][c] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet)
                    newNode.moveSet.append([r,c,'U'])
                    neighboringStates.append(newNode)
                
                # Checking to the right
                if ((c+2) < cMax and stateNode.state[r][c+2] == 0 and stateNode.state[r][c+1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r][c+2] = 1
                    newNode.state[r][c+1] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet) 
                    newNode.moveSet.append([r,c,'R'])
                    neighboringStates.append(newNode)

                # checking to the left
                if ((c-2) >= 0 and stateNode.state[r][c-2] == 0 and stateNode.state[r][c-1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r][c-2] = 1
                    newNode.state[r][c-1] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet) 
                    newNode.moveSet.append([r,c,'L'])
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
    visited = set() # array of visited and visiting states
    maxMove = 0

    # queue holding states to expand
    queue = deque()
    queue.append(InitNode)
    while len(queue) > 0:
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
    Open = SortedLinkedList()
    Closed = set()

    Open.AddNode(InitNode)
    while Open.counter > 0:
        # Pop the top
        N = Open.Pop()
        
        t = StateToTuple(N.state)
        Closed.add(t)

        if Goal(N.state):
            return N
        
        ne = Successor(N)
        for n in ne:
            t = StateToTuple(n.state)
            if t not in Closed:
                n.f = Heuristic(n.state)
                Open.AddNode(n)
    
    return False


def A_Star_Search(InitNode):
    if Goal(InitNode.state):
        return InitNode

    Closed = set() # array of visited and visiting states
    Open = SortedLinkedList()
    Open.AddNode(InitNode)

    while Open.counter > 0:
        q = Open.Pop()
        neighbors = Successor(q)

        t = StateToTuple(q.state)
        Closed.add(t)
        for n in neighbors:
            # convert to tuple
            t = StateToTuple(n.state)

            if t not in Closed:
                if Goal(n.state):
                    return n
                n.f = Heuristic(n.state) + len(n.moveSet)
                
                # for i in range(Open.counter):
                #     if n.state == Open.state:
                #         if len(n.moveSet) < len(Open[i].moveSet):
                #             Open[i] = n

                #         FOUND = True
                #         break

                Open.AddNode(n)
    return False
    # end of WHILE
                    
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
        if i[2] == 'D':
            initState[r+2][c] = 1
            initState[r+1][c] = initState[r][c] = 0
        elif i[2] == 'U':
            initState[r-2][c] = 1
            initState[r-1][c] = initState[r][c] = 0
        elif i[2] == 'R':
            initState[r][c+2] = 1
            initState[r][c+1] = initState[r][c] = 0
        else:
            initState[r][c-2] = 1
            initState[r][c-1] = initState[r][c] = 0

        print('Move',count)
        for j in initState:
            print(j)
        
        print('')

# FOR analysis. It tracks how much memory is used and process duration
def Memory_Time_Keeper(File):
    curr_mem, max_memory = tracemalloc.get_traced_memory()
    memory_usage_refresh = 1 #.005 # Seconds

    # 
    while(2*curr_mem / 10**6 < 2800.0 and time.process_time() < 7200.0):
        time.sleep(memory_usage_refresh)
        curr_mem, max_memory = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    File.write("T,0," + str(time.process_time()) + "," + str(2*curr_mem / 10**6) + "," + str(2*max_memory / 10**6) + "\n")
    File.close()
    # print("Termination")
    # print("Memory", curr_mem / 10**6)
    # print("Time", time.process_time())
    os._exit(0)

def ExperimentState(argu):
    cMax = rMax = argu
    state = []
    for r in range(rMax):
        temp = []
        for c in range(cMax):
            temp.append(1)
        state.append(temp)

    
    if argu == 4 or argu == 5:
        state[1][0] = 0
    elif argu == 8:
        state[4][2] = 0
    elif argu == 9: 
        state[4][4] = 0
    elif argu == 10:
        state[5][4] = 0
    else:
        state[1][1] = 0
    
    return state


if __name__ == "__main__":
    # if str(sys.argv[2]) == "BFS" or str(sys.argv[2]) == "DFS": 
    #     File = open("DFS_BFS_Experiments.txt", "a")
    # elif str(sys.argv[2]) == "GBS":
    #     File = open("GBS_Experiments.txt", "a")
    # else: 
    #     File = open("Astar_Experiments.txt", "a")
    
    # # starting the tracking of memory usage
    # tracemalloc.start()

    # # creating node object
    # InitNode = Node()
    # InitNode.state = ExperimentState(int(sys.argv[1]))  # initializing pegboard 

    # # # letting global size variables
    # cMax = len(InitNode.state[0])
    # rMax = len(InitNode.state)

    # # starting time and memory keeper
    # pthread = threading.Thread(target=Memory_Time_Keeper, args=(File,), daemon=True)
    # pthread.start()
    
    # # Search algorithm (DFS, BFS, GBS, Astar), NxN,
    # Search = str(sys.argv[2])
    # File.write(Search + "," + str(sys.argv[1]) + "x" + str(sys.argv[1]) + ",")
    # print("Starting w/" + Search + "," + str(sys.argv[1]) + "x" + str(sys.argv[1]))

    # # For experimentation
    # if Search == "DFS":
    #     path = Depth_First_Search(InitNode)
    # elif Search == "BFS":
    #     path = Breadth_First_Search(InitNode)
    # elif Search == "GBS":
    #     path = Greedy_Best_Search(InitNode)
    # else:
    #     path = A_Star_Search(InitNode)
    
    # # if a solution was found
    # if path != False and Goal(path.state):
    #     # PathPrint(InitNode.state, path.moveSet)
    #     File.write("S," + str(len(path.moveSet)) + ",") # solved (S), #moves
    # else:
    #     File.write("F,0,")  # failed (F), #move = 0

    # curr_mem, max_mem = tracemalloc.get_traced_memory()
    # # save execution time, curr memory and max memory usage 
    # File.write(str(time.process_time()) + "," + str(2*curr_mem / 10**6) + "MB," + str(2*max_mem/ 10**6) + "MB\n")
    # File.close()
    # print("DENE w/" + Search + "," + str(sys.argv[1]) + "x" + str(sys.argv[1]) +"\n\n")
    
    InitNode = Node()
    
    # initializing pegboard 
    InitNode.state = ExperimentState(9)

    # # letting global size variables
    cMax = len(InitNode.state[0])
    rMax = len(InitNode.state)

    path = A_Star_Search(InitNode)
    PathPrint(ExperimentState(9), path.moveSet)