from copy import deepcopy 
from collections import deque

cMax = 0
rMax = 0
class Node:
    state = []
    moveSet = []
    f = -1

# Manhattan Algorithm
def Heuristic(state):
    sum = 0
    for r in range(len(state)):
        for c in range(len(state[0])):
            if state[r][c] != 0:
                sum = sum + abs(r - len(state)) + abs(c - len(state[0]))
    return sum

# Goal determines whether given state is solution
def Goal(state):
    sum = 0
    for r in range(len(state)):
        for c in range(len(state[0])):
            sum = sum + state[r][c] 
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
                    newNode.moveSet.append('D')
                    neighboringStates.append(newNode)

                # Checking Above
                if ((r-2) >= 0 and stateNode.state[r-2][c] == 0 and stateNode.state[r-1][c] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r-2][c] = 1
                    newNode.state[r-1][c] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet)
                    newNode.moveSet.append('U')
                    neighboringStates.append(newNode)
                
                # Checking to the right
                if ((c+2) < cMax and stateNode.state[r][c+2] == 0 and stateNode.state[r][c+1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r][c+2] = 1
                    newNode.state[r][c+1] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet) 
                    newNode.moveSet.append('R')
                    neighboringStates.append(newNode)

                # checking to the left
                if ((c-2) >= 0 and stateNode.state[r][c-2] == 0 and stateNode.state[r][c-1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(stateNode.state)
                    newNode.state[r][c-2] = 1
                    newNode.state[r][c-1] = newNode.state[r][c] = 0
                    newNode.moveSet = deepcopy(stateNode.moveSet) 
                    newNode.moveSet.append('L')
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

# input is Node
def Greedy_Best_Search(InitNode):
    Open = []
    Closed = set()
    Open.append(InitNode)
    while len(Open) > 0:
        # Pop the top
        N = Open.pop(0)
        
        t = StateToTuple(N.state)
        Closed.add(t)

        if Goal(N.state):
            return N
        
        ne = Successor(N)
        for n in ne:
            t = StateToTuple(n.state)
            if t not in Closed:
                n.f = Heuristic(n.state)
                Open.append(n)

        if len(ne) > 0:
            # To sort the list in place...
            Open.sort(key=lambda x: x.f, reverse=False)
    
    return False


def A_Star_Search(InitNode):
    Closed = set() # array of visited and visiting states
    Open = []
    Open.append(InitNode)

    while len(Open) > 0:
        q = Open.pop(0) 
        neighbors = Successor(q)

        t = StateToTuple(q.state)
        Closed.add(t)
        for n in neighbors:
            if Goal(n.state):
                return n

            # convert to tuple
            t = StateToTuple(n.state)

            if t not in Closed:
                n.f = Heuristic(n.state) + len(n.moveSet)
                FOUND = False
                for i in range(len(Open)):
                    if n.state == Open[i].state:
                        if len(n.moveSet) < len(Open[i].moveSet):
                            Open[i] = n

                        FOUND = True
                        break

                if not FOUND:
                    Open.append(n)

        # sort by lowest f
        Open.sort(key=lambda x: x.f, reverse=False)

    # end of WHILE
                    
            

if __name__ == "__main__":
    # creating node object
    InitNode = Node()

    # initializing pegboard 
    InitNode.state = [[1,1,1,1,1,1],
                    [1,0,1,1,1,1],
                    [1,1,1,1,1,1],
                    [1,1,1,1,1,1],
                    [1,1,1,1,1,1],
                    [1,1,1,1,1,1]]
    
    # letting global size variables
    cMax = len(InitNode.state[0])
    rMax = len(InitNode.state)
                          
    path = A_Star_Search(InitNode)
    print(path)