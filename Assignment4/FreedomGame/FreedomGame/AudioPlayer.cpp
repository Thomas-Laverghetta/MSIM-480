#include "AudioPlayer.h"

/* Plays music in the background while playing TicTacToe
	
	To use any of these funcitons, you have to make sure that your
	Compiler as 'winmm.lib' in the linker->input->Additional Dependencies
*/

// Jeopardy song in the back ground for menue
void BackGroundMusic() {
	// Opens MP3 with Jeopardy
	mciSendString(TEXT("open \"Jeopardy-theme-song.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);

	// Plays Jeopardy with it repeating everytime it ends
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);
}

// Pokemon Battle song
void BattleMusic()
{
	// Opens the Pokemon Battle song file
	mciSendString(TEXT("open \"107-battle (vs wild pokemon).mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	
	// Play pokemon with a repeat
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);
}

// If user wins
void Victory()
{
	// Plays Pokemon Victory song
	mciSendString(TEXT("open \"116-victory (vs trainer).mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);
}

// If Cat wins
void CatVictory() {
	// Plays Nyan Cat
	mciSendString(TEXT("open \"NyanCatoriginal.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);
}

// if Computer Wins
void CompWin() {
	// Plays Knife Part 404
	mciSendString(TEXT("open \"FDM_Knife_Party_-_404_320_kbps_(mp3.pm).mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);
}

// Playing "That's All Folks" for outtro 
void ThatAllFolks()
{
	mciSendString(TEXT("open \"Thats All Folks.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3"), NULL, 0, NULL); // no repeating
}



// Clear the song current playing in the main
void Clear() {
	// Stop the current song
	mciSendString(TEXT("stop mp3"), NULL, 0, NULL);

	// Closes the MP3 file
	mciSendString(TEXT("close mp3"), NULL, 0, NULL);
}


