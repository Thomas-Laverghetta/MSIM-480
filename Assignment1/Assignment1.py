from copy import deepcopy 

class Node:
    state = []
    Prestate = []
    f = -1
    g = -1
    h = -1

# Manhattan Algorithm
def Heuristic(state):
    sum = 0
    for r in range(len(state)):
        for c in range(len(state[0])):
            if state[r,c] != 0:
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
def Successor(state):
    neighboringStates = []
    for r in range(len(state)):
        for c in range(len(state[0])):
            if (state[r][c] != 0):
                # Checking Above
                if ((r+2) < len(state) and state[r+2][c] == 0 and state[r+1][c] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(state)
                    newNode.state[r+2][c] = 1
                    newNode.state[r+1][c] = newNode.state[r][c] = 0
                    neighboringStates.append(newNode)

                # Checking Below
                if ((r-2) >= 0 and state[r-2][c] == 0 and state[r-1][c] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(state)
                    newNode.state[r-2][c] = 1
                    newNode.state[r-1][c] = newNode.state[r][c] = 0
                    neighboringStates.append(newNode)
                
                # Checking to the right
                if ((c+2) < len(state[0]) and state[r][c+2] == 0 and state[r][c+1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(state)
                    newNode.state[r][c+2] = 1
                    newNode.state[r][c+1] = newNode.state[r][c] = 0
                    neighboringStates.append(newNode)

                # checking to the left
                if ((c-2) >= 0 and state[r][c-2] == 0 and state[r][c-1] == 1):
                    newNode = Node()
                    newNode.state = deepcopy(state)
                    newNode.state[r][c-2] = 1
                    newNode.state[r][c-1] = newNode.state[r][c] = 0
                    neighboringStates.append(newNode)
    
    return neighboringStates

def StateSum(state):
    sum = 0
    for r in range(len(state)):
        for c in range(len(state[0])):
            sum = sum + state[r][c]

    return sum 

# input is a node
def Breadth_First_Search(InitNode):
    index = StateSum(InitNode.state) - 1
    closed = [[]]
    queue = []
    queue.append(InitNode)
    # closed[0].append(InitNode.state)
    while len(queue) > 0:
        c = queue.pop(0)         # Get next element of queue
        if Goal(c.state):       # if current state is goal
            # Find the nodePath to solution
            nodePath = []
            nodePath.append(c)  
            it = c
            while len(it.Prestate) > 0:
                nodePath.append(it.Prestate)
                it = it.Prestate
            
            return nodePath
        else:
            neighbors = Successor(c.state)
            for n in neighbors:
                unvisited = True
                sumIndex = index - StateSum(n.state)
                if sumIndex < len(closed):
                    for j in range(len(closed[sumIndex])):
                        if closed[sumIndex][j] == n.state:
                            unvisited = False
                            break
                    if unvisited:   # unvisited
                        closed[sumIndex].append(n.state)
                        n.Prestate = c.state # The previous state (used to find state path)
                        queue.append(n) # add to queue
                else:
                    closed.append([n.state])
                    n.Prestate = c.state # The previous state (used to find state path)
                    queue.append(n) # add to queue 
    
    return False
    
# input is a node
def Depth_First_Search(InitNode):
    if Goal(InitNode):
        # Find the nodePath to solution
        nodePath = []
        nodePath.append(InitNode)  
        it = InitNode
        while len(it.Prestate) > 0:
            nodePath.append(it.Prestate)
            it = it.Prestate
        
        return nodePath
    else:
        InitNode.f = 1 # mark visited
        neighbors = Successor(InitNode.state)
        for n in neighbors:
            if n.f == -1:
                n.Prestate = InitNode
                Depth_First_Search(n)

# input is Node
def Greedy_Best_Search(InitNode):
    Open = []
    Closed = []
    Open.append(InitNode)
    while len(Open) > 0:
        # Pop the top
        N = Open.pop(0)
        Closed.append(N)

        if Goal(N.state):
            Closed.append(N)
            return Closed
        
        Open.append(Successor(N.state))
        H = []
        for n in range(len(Open)):
            H.append(Heuristic(Open[n].state))

        ## bubble sort
        n = len(H)
  
        # Traverse through all array elements 
        for i in range(n-1): 
        # range(n) also work but outer loop will repeat one time more than needed. 
    
            # Last i elements are already in place 
            for j in range(0, n-i-1): 
    
                # traverse the array from 0 to n-i-1 
                # Swap if the element found is greater 
                # than the next element 
                if H[j] > H[j+1] : 
                    H[j], H[j+1] = H[j+1], H[j] 
                    Open[j], Open[j+1] = Open[j+1], Open[j]
    
    return False



def A_Star_Search(InitNode):
    Open = []
    Closed = []
    Open.append(InitNode)

    while len(Open) > 0:
        q = Open.pop(0) 
        neighbors = Successor(q.state)
        for n in neighbors:
            if Goal(n):
                return True
                break
            
            n.g = q.g + 1
            n.h = Heuristic(n.state)
            n.f = n.g + n.h
            
            c = True
            for i in range(len(Open)):
                if StatesEqual(Open[i].state, n.state):
                    if Open[i].f < n.f:
                        c = False
            
            if c:
                for i in range(len(Closed)):
                    if StatesEqual(Closed[i].state, n.state):
                        if Closed[i].f >= n.state:
                            Open.append(n)
                            break
            
        Closed.append(q)
    # end of WHILE
                    
            

if __name__ == "__main__":
    InitNode = Node()

    InitNode.state = [[1,1,1,1,1,1],[1,0,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1]]

    path = Breadth_First_Search(InitNode)
    print(path)