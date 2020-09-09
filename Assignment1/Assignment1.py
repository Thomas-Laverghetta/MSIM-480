

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
            sum = sum + state[r,c] 
    return sum == 1 # TRUE if final state

# Successor determines naighboring states
def Successor(state):
    neighboringStates = []
    for r in range(len(state)):
        for c in range(len(state[0])):
            if (state[r,c] != 0):
                # Checking Above
                if ((r+2) < len(state) and state[r+2, c] == 0):
                    newNode = Node()
                    newNode.state = state
                    newNode.state[r+2, c] = 1
                    newNode.state[r+1, c] = newNode.state[r, c] = 0
                    neighboringStates.append(newNode)

                # Checking Below
                if ((r-2) >= 0 and state[r-2, c] == 0):
                    newNode = Node()
                    newNode.state = state
                    newNode.state[r-2, c] = 1
                    newNode.state[r-1, c] = newNode.state[r, c] = 0
                    neighboringStates.append(newNode)
                
                # Checking to the right
                if ((c+2) < len(state[0]) and state[r, c+2] == 0):
                    newNode = Node()
                    newNode.state = state
                    newNode.state[r, c+2] = 1
                    newNode.state[r, c+1] = newNode.state[r, c] = 0
                    neighboringStates.append(newNode)

                # checking to the left
                if ((c-2) >= 0 and state[r, c-2] == 0):
                    newNode = Node()
                    newNode.state = state
                    newNode.state[r, c-2] = 1
                    newNode.state[r, c-1] = newNode.state[r, c] = 0
                    neighboringStates.append(newNode)
    
    return neighboringStates

# input is a node
def Breadth_First_Search(InitNode):
    queue = []
    queue.append(InitNode)
    while len(queue) > 0:
        c = queue.pop()         # Get next element of queue
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
                if n.f == -1:   # unvisited
                    n.f = 1     # mark as visited
                    n.Prestate = c.state # The previous state (used to find state path)
                    queue.append(n) # add to queue
    
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

# input is state
def Greedy_Best_Search(InitNode):
    Open = []
    Closed = []
    Open.append(InitNode)
    while True:
        if len(Open) == 0:
            return nodePath = []

        # Pop the top
        N = Open.pop()
        Closed.append(N)

        if Goal(N):
            Closed.append(N)
            return Closed
        
        Open.append(Successor(N))
        H = []
        for n in range(len(Open)):
            H.append(Heuristic(Open[n]))

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




        
