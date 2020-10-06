# First Assignment - Pegboard Game Solver 
The program's objectives are given a pegboard game find a solution (i.e., only one peg left on the board). 

Rules of the game. Pegboard problems are single-player games played on a grid, in which moves are made by successively jumping and removing pegs from the pegboard. A peg can jump an adjacent peg if there is a slot adjacent to that peg in the opposite direction – horizontally or vertically. Diagonal jumps are not allowed. After a peg has been jumped, it is removed from the board (and possibly eaten). A typical objective of this problem is to begin with a full pegboard from which one peg has been removed and determine a sequence of jumps which will result in one peg remaining. 

# System State
To represent the system (pegboard game), system states will be a matrix NxN where a element represents a peg slot, 1 represents a peg in slot, and 0 represents no peg in slot. E.g., 
```
0111
1111
1111
1111
```

## Implementation
There are three methods: 
- goal(state): returns TRUE if state equals the state with exactly 1-peg
- successor(state): given a state, return all possible state from current state 
- heuristic(state): given a state, return heuristic function value

## Heuristics Algorithm
Using Manhattan algorithm
```
Heuristic(state):
    sum = 0
    For r in len(state[:,]):
        For c in len(state[,:]):
            if (state[r,c] != 0):
                sum = sum + | r - len(state[:,]) / 2 | + | c - len(state[,:]) / 2 |

    Return sum  
```

## Goal Method
```
Goal(state):
    sum = 0
    For r in len(state[:,]):
        For c in len(state[,:]):
            sum = sum + state[r,c] 
    Return sum == 1 
```
## Successor Method
```
Successor(state):
    create empty list 
    find all possible moves
    for each move:
        calculate state based on move
        add state to list
    Return list
```

## Algorithms
For this assignment, I am using the following algorithms. 
- Breadth-first search
- Depth-first search
- Greedy-best search
- A* search

## Breadth-first search Algorithm (BFS)
```
Bread First Search (State S)
1. create FIFO Queue
2. push S into Queue 
3. While Queue Is Not Empty
4. 	T = Pop Queue
5. 	if T is the goal
6.		return T
7.	else
8.		foreach successor of T
9.			if successor state has not been visited
10.				mark successor state has visited
11.				push successor into Queue
12.			end if
13.		end for
14.	end if
15. end while
16. return no solution found
```

## Depth-first search Algorithm (DFS)
```
Depth First Search (State S)
1. if S is the goal
2. 	mark S has goal
3.	return S
4. else
5. 	foreach successor of S 
6. 		if successor state has not been visited
7.			mark successor state has visited
8.			if Depth First Search (S) is the goal
9.				return Depth First Search (S)
10.			end if
11.		end if
12. 	end for
13. end if
```

## Greedy Best Search Algorithm (GBS)
```
Greedy Best Search (State S)
1. create FIFO Queue
2. push S into Queue
3. While Queue Is Not Empty
4. 	T = Pop Queue
5. 	if T is the goal
6.		return T
7.	end if
8.	foreach successor of T
9.		if successor state not visited
10.			mark successor state has visited
11.			calculate and save heuristic for successor
12.			push successor into Queue
13.		end if
14.	end for
15.	sort Queue in ascending order based on heuristic scores 
16. end while
17. return no solution found 	
```

## A* Search Algorithm (Astar)
```
A* (State S)
1. if S is the goal
2. 	return S
3. end if
4. create FIFO Queue
5. push S into Queue 
6. While Queue Is Not Empty
7. 	T = Pop Queue
8.	foreach successor of T
9.		if successor state has not been visited
10.			if successor is the goal
11. 				return successor
12.		 	end if
13.			calculate and save heuristic and cost function for successor
14.			if successor state in Queue
15.				if successor’s cost is less than equivalent state in Queue’s cost
16.					replace equivalent state with successor in Queue
17.				end if
18.			 else
19.				push successor into Queue
20.			end if
21.		end if
22.	end for
23.	sort Queue in ascending order based on heuristic plus cost scores
24. end while
25. return no solution found

```

## Data Structures
```
Node:
    node state   
    heuristic function f = -1
END
```

## How to Use Program
To use the program you must choose your algorithm to run (BFS, DFS, GBS, or Astar) and your input pegboard using system state representation above. Inputs are added to input.csv file where the first line of the file is algorithm and following lines (2-) is initial system state. E.g. Using Astar and 4x4 initial state:
```
Astar
1,0,1,1
1,1,1,1
1,1,1,1
1,1,1,1
```

Once input are defined, user can just execute python script (Assignment1.py) and observe outputs (solution path).
