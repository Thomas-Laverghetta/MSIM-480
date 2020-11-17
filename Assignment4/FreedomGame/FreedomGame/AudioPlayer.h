#pragma comment(lib, "winmm.lib")
// Audio header 
// Audio Libraries
#include <iostream>
#include <Windows.h>
#include <mciapi.h>
#include "MMSystem.h"

using namespace std;

void Victory(); // User win song 
void BackGroundMusic(); // Background for menue
void BattleMusic();// song for When playing TicTactoe 
void Clear();// Clearing anysong playing
void CatVictory();// If Cat Wins
void CompWin();// If the computer Win
void ThatAllFolks(); // out tro