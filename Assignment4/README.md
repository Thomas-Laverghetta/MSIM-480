## Freedom Board Game AI Program
This program will let human players play against a AI using minimax alpha-beta pruning progressive depth algorithm.

# Freedom Board Game
“Freedom is a two-player abstract strategy board game invented by Veljko Cirovic and Nebojsa Sankovic in 2010. It is played with black and white stones on a square board. The game is related to Go-Moku and Slimetrail. It can be played with a Go set or with pen and paper.

Rules:

Board: Freedom is played on a 10×10 square board. Beginners can try the game on a 8×8 board. Other board sizes may be also used.

Objective: The objective of Freedom is to have more "live" stones at the end of the game, than your opponent. A stone is considered to be "live" if it is a part of some horizontal, vertical or diagonal row of exactly 4 stones of the same color.

Play: A game begins with an empty board. Each player has an allocated color: White and Black. White plays first, putting one white stone anywhere on the board. After this move players take turns placing their stones on empty cells adjacent to the last opponent's stone. If all cells adjacent to the last opponent's stone are occupied then the player gets the right ("freedom") to place his stone on any empty cell of the board. The game ends when the board is filled with stones. The last player has the right to pass on his last turn (and leave the last cell empty) if placing his stone reduces his score.”

https://boardgamegeek.com/boardgame/100480/freedom

# How to Compile Program
Program is programmed in C++, therefore, there are two ways to compile and run. One, use visual studio and open FreedomGame/FreedomGame.sln with visual studio to compile and run. Or use favorite C++ compiler by compiling source code under FreedomGame/FreedomGame.

# How to Run Program
Once compiler, run exe. You will be presented a user-prompt to choose stone color white or black (W or B, respectively). Then pending on color chosen, either the AI or you will start by playing a stone (white goes first). The game will continue until a player has won. At which a victory song will play. 