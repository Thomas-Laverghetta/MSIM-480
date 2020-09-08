# First Assignment - Pegboard Game Solver 
The program's objectives are given a pegboard game find a solution (i.e., only one peg left on the board). 

Rules of the game. Pegboard problems are single-player games played on a grid, in which moves are made by successively jumping and removing pegs from the pegboard. A peg can jump an adjacent peg if there is a slot adjacent to that peg in the opposite direction – horizontally or vertically. Diagonal jumps are not allowed. After a peg has been jumped, it is removed from the board (and possibly eaten). A typical objective of this problem is to begin with a full pegboard from which one peg has been removed and determine a sequence of jumps which will result in one peg remaining. 

# System State
To represent the system (pegboard game), system states will be a matrix MxN where a element represents a peg slot, 1 represents a peg in slot, and 0 represents no peg in slot. E.g., 

--
0111
1111
1111
1111
--


## Implementation
There are three methods: 
- goal(state): returns TRUE if state equals the state with exactly 1-peg
- successor(state): given a state, return all possible state from current state 
- heuristic(state): given a state, return heuristic function value

## Heuristics Algorithm
Using Manhattan algorithm
--
Heuristic(state):
    sum = 0
    For r in len(state[:,]):
        For c in len(state[,:]):
            if (state[r,c] != 0):
                sum = sum + | r - len(state[:,]) / 2 | + | c - len(state[,:]) / 2 |

    Return sum  
--

## Goal Method
--
Goal(state):
    sum = 0
    For r in len(state[:,]):
        For c in len(state[,:]):
            sum = sum + state[r,c] 
    Return sum == 1 
--

## Algorithms
For this assignment, I am using the following algorithms. 
- Breadth-first search
- Depth-first search
- Greedy-best search
- A* search

## Breadth-first search Algorithm
--
Breadth-First-Search( Maze m )
    EnQueue( m.StartNode )
    While Queue.NotEmpty 
        c <- DeQueue
        If c is the goal
            Exit
        Else
            Foreach neighbor n of c
                If n "Unvisited"
                    Mark n "Visited"                    
                    EnQueue( n )
            Mark c "Examined"                    
End procedure
--

Reference: https://courses.cs.washington.edu/courses/cse326/03su/homework/hw3/bfs.html 

## Depth-first search Algorithm
Depth-First-Search-Kickoff( Maze m )
    Depth-First-Search( m.StartCell )
End procedure

Depth-First-Search( MazeCell c )
    If c is the goal
        Exit
    Else
        Mark c "Visit In Progress"
        Foreach neighbor n of c
            If n "Unvisited"
                Depth-First-Search( n )
        Mark c "Visited"
End procedure

Reference: https://courses.cs.washington.edu/courses/cse326/03su/homework/hw3/dfs.html 

## Greedy Best Search Algorithm
1. Create 2 empty lists: OPEN and CLOSED
2. Start from the initial node (say N) and put it in the ‘ordered’ OPEN list
3. Repeat the next steps until GOAL node is reached
    4. If OPEN list is empty, then EXIT the loop returning ‘False’
    5. Select the first/top node (say N) in the OPEN list and move it to the CLOSED list. Also capture the information of the parent node
    6. If N is a GOAL node, then move the node to the Closed list and exit the loop returning ‘True’. The solution can be found by backtracking the path
    7. If N is not the GOAL node, expand node N to generate the ‘immediate’ next nodes linked to node N and add all those to the OPEN list
    8. Reorder the nodes in the OPEN list in ascending order according to an evaluation function (heuristic)
7. RETURN findings

Reference: https://www.mygreatlearning.com/blog/best-first-search-bfs/#:~:text=The%20Greedy%20BFS%20algorithm%20selects,take%20advantages%20of%20both%20algorithms

## A* Search Algorithm
1.  Initialize the open list
2.  Initialize the closed list
    put the starting node on the open 
    list (you can leave its f at zero)

3.  while the open list is not empty
    a) find the node with the least f on 
       the open list, call it "q"

    b) pop q off the open list
  
    c) generate q's 8 successors and set their 
       parents to q
   
    d) for each successor
        i) if successor is the goal, stop search
          successor.g = q.g + distance between 
                              successor and q
          successor.h = distance from goal to 
          successor (This can be done using many 
          ways, we will discuss three heuristics- 
          Manhattan, Diagonal and Euclidean 
          Heuristics)
          
          successor.f = successor.g + successor.h

        ii) if a node with the same position as 
            successor is in the OPEN list which has a 
           lower f than successor, skip this successor

        iii) if a node with the same position as 
            successor  is in the CLOSED list which has
            a lower f than successor, skip this successor
            otherwise, add  the node to the open list
     end (for loop)
  
    e) push q on the closed list
    end (while loop) 

Reference: https://www.geeksforgeeks.org/a-search-algorithm/ 

## Data Structures
--
Node:
    state
    f = -1
    g = -1
    h = -1
END
--

## Functional Implemnentation
