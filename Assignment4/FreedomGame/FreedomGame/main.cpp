/* 
 Date: 11/17/2020
 Name: Thomas Laverghetta 
 Project: Assignment 4
*///**********************************************************************************


#include "TicTacToe.h"
#include "AudioPlayer.h"

int main()
{
	// Changing the color of the exe window
	system("color f0");
	
	cout << "\tPLEASE HAVE AUDIO ON!!! There is Music Playing in the background. \n\n";

	// Declaring the variables to use in the main
	int player, Choice;
	int Game;
	int AI;
	bool go, Players;
	char WHY;

// goto statement
Play:
	// Clearing any music that might be playing in the back ground
	Clear();

	// Starting the back gound music
	BackGroundMusic();

	// initalizing the two players
	TicTacToe Plays;
	do {
		/* Prompt for method of the game*/
		cout << "Player Vs Player, or Player Vs Compuer? (1 or 2): ";
		cin >> AI;
		cin.ignore();

		// Error checking player input
		if (AI <= 0 || AI >= 3) {
			cout << "Error, invalid input, please try again: \a\n"; // there's a bell
		}
	} while (AI <= 0 || AI >= 3); // if invalid input loop

	// Player Vs Player
	if (AI == 1) {
		// Introduction...
		// Players enter thier names 
		Plays.Enter_Player_Name(0, false); // 0 = player one
		Plays.Enter_Player_Name(1, false); // 1 = player two
	}
	// Computer Vs Player 
	else if (AI == 2) {
		Plays.Enter_Player_Name(0, false); // PLayer
		Plays.Enter_Player_Name(1, true); // computer
	}
	// seeing if the user wants to choose who goes first or if they want to choose 
	do {
		cout << "Do you wnat to choose who goes first (1) or Let the computer Choose (2): ";
		cin >> Choice;
		// Who plays first?

		// Player chooses
		if (Choice == 1) {
			do {
				cout << "\nWhat person do you want first Player 1 or 2?: ";
				cin >> player;
				// making sure place in right value
				if (player <= 0 || player >= 3) {
					cout << "\nERROR, invalid input, please try again \a\n"; // bell
					cin.clear();
					cin.ignore();
					Players = false;
				}
				else {
					Players = true;
					player--; // Since player can only be 1 or 0 however the user is allowed to place in 2 or 1
				}
			} while (!Players); // loop if players is false
			go = true;
		}

		// Computer Chooses
		else if (Choice == 2) {
			player = Plays.First_Player();
			go = true;
		}
		// Error with input
		else {
			go = false;
			cout << "Error, Invalid input, Please try Again: \n\a"; // bell
		}
	} while (!go); // loop if go is false

	Clear(); // Stop Jeopordy music
	BattleMusic(); // Start Battle Music

	for (; ; ) {

		// PLayer enter their moves
		Plays.Make_Move(player);

		Game = Plays.Game_Over(player);
		if (Game == 1) {
			// Asking if the computer Playing (Player Vs Computer)
			if (AI == 2) {
				// if Computer won than play music for computer
				if (player == 1) {
					Plays.Display_Board(); // displaying baord
					Clear(); // clearing the last song
					CompWin(); // Play computer win song
					cout << " Won! GG!\n\n";
				}
				// IF the user wins against the computer
				else {
					Plays.Display_Board(); // displaying the baord
					Plays.Display_Player_Name(player); // displaying the player
					Clear(); // clearing the last song
					Victory(); // Play victory song for user
					cout << " Won! GG! \n\n";
				}
			}
			// If Player Vs Player
			else{
				// displaying the board
				Plays.Display_Board();

				// displaying players name
				Plays.Display_Player_Name(player);

				// stopping the music
				Clear();

				// starting playing the Player Victory song
				Victory();

				// display win!
				cout << " Won! GG!\n\n";
			}
			break; // break for win
		}
		// if the Cat Won
		else if (Game == 2) {
			// Stop the current song
			Clear();

			// start playing Cat song
			CatVictory();

			// display board
			Plays.Display_Board();

			// display that the cat won
			cout << "The CAT Won! DRAW GG!\n\n";
			break; // break for win
		}
		/*
		Next player
		0 or 1
		*/
		if (player == 0) player = 1;
		else if (player == 1) player = 0;
	}
	bool WHYY; // for error for playing again
	do {
		// prompting the user if they want to play again
		cout << "Would you like to play Again? (Y or N): ";
		cin >> WHY;
		if (ListMatcher(WHY, { 'y','Y' })) {
			goto Play; // goto play statement above
		}
		else if (ListMatcher(WHY, { 'n','N' })) {
			cout << "\nCool\n";
			Clear(); // stopping current song
			ThatAllFolks(); // Playing outTro
			WHYY = true;
		}
		else {
			cout << "\nInvalid Input. Try Again: \a\n"; // bell
			WHYY = false; // Loop becuase false
		}
	} while (!WHYY); // for detecting error in user input
					 // Loop if WHYY = false
	system("pause");
	return 0;
}