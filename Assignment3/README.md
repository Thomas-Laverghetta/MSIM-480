## Crossword Puzzle Solver
The programs in this folder (task 1 and 2) have backtracking algorithms for solving crossword puzzles. Task 1 is a basic backtracking algorithm which takes a user difined dictionary (word list) and user defined crossword. Task 2 uses a heuristic backtracking algorithm and takes a user defined crossword puzzle and optional user defined dictionary.

# How Compile
The programs are programmed in C++. Thus, users must compile program either using Visual Studio SLN files (provided) or using prefered C++ compiler with source files. 

# How to Run
Task 1 & 2 use XML files to provide crossword puzzle input. The XML are format for Task 1 program is has follows:
```
<CrosswordElement elementId="<Element ID>" row="<element row>" col="<element col>" direction="<H or V (horizontal or vertical)>" size="<length of word>"></CrosswordElement>
```

Task 2's is has follows:
```
<CrosswordElement elementId="<Element ID>" row="<element row>" col="<element col>" direction="<H or V (horizontal or vertical)>" size="<length of word>" letterIndex="<default -1 for no letter>" (OPTIONAL)letter="<letter to prune dictionary with>"></CrosswordElement>
```
Task 2 allows for users to give letter constraints by inputting a letter and where it is relative to the element. If element does not have constraint, then user will put -1 place holder for letter index (signals program there is no letter constraint for element). Else, give the user will put letter index and the letter (upper case).

For examples, users can see task 2's tree or heart crossword XML based on system requirement crossword puzzles. Also, for crossword elements with horizontal and vertical components, users will create two crossword elements one with positive element Id and another with negative element Id where positive will have horizontal or vertical and negative will have the other. 

To input XML to run, goto main function and changed parameter of LoadWordRestrictions to name of XML file.

Task 1 has a user defined dictionary which can be modified by modifying dictionary.txt file in Task 1 folder. 