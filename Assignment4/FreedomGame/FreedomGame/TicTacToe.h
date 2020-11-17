#pragma once
// TicTacToe Header
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <fstream>
#include <initializer_list>

using namespace std;

// Class
class TicTacToe {
private:
	// Size of the TicTacToe Board
	static const int Size = 3; 

	// Variables to hold player moves
	int rowMove, colMove; 

	// Player's move A->C
	char CBA; 

	// Holding Player's name
	string Player[2]; 

	// Board to hold player moves
	char BoardMoves[Size][Size]; 

	// Map to convert string to char
	map <string, char> Plays; 

	// Map to convert char to int
	map <char, int> ABC;

	// int to char
	map <int, char> ReverseABS; 

public:
	// defualt constructor
	TicTacToe(); 

	// random player to play first
	int First_Player(); 

	// Player enters thier name and whether Computer is playing
	void Enter_Player_Name(int N, bool AI); 

	// Display Player N
	void Display_Player_Name(int N); 
	
	// Prompt player to play a move
	void Make_Move(int N); 

	// Validating move made be player
	int Valid_Move(); 

	// displays the board with moves
	void Display_Board(); 

	// Determining if the game is over (someone won)
	int Game_Over(int N); 

	// Computer Makes a move using RNG
	void AI_Moves(int& row, int& col); 

};

// Prototype for ListMatcher
template <typename T>
bool ListMatcher(const T& val, const initializer_list<T>& list);


