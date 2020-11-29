#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "AudioPlayer.h"
#define MAX_ROW 10
#define MAX_COL 10
#define MAX_SCORE INT_MAX
#define MIN_SCORE INT_MIN
typedef int score;
using namespace std;


// Symbols for AI and human player
char AI_sym;
char Human_sym;

// node represents game board
struct Node {
    uint8_t lastPlay[2];    // last play to create current board. row, col
    char currBoard[MAX_ROW][MAX_COL];
    Node() {
        lastPlay[0] = MAX_ROW;
        lastPlay[1] = MAX_COL;
        for (uint8_t r = 0; r < MAX_ROW; r++) {
            for (uint8_t c = 0; c < MAX_COL; c++) {
                currBoard[r][c] = '\0';
            }
        }
    }
};

score Max(const score& a, const score& b) {
    if (a > b)
        return a;
    else
        return b;
}
score Min(const score& a, const score& b) {
    if (a < b)
        return a;
    else
        return b;
}

// returns a set of possible moves
vector<Node> PossibleMoves(const Node& node, const char& player) {
    vector<Node> nodeSet;

    // if initial value (when AI is white), then all empty spaces are possible plays
    if (node.lastPlay[0] == MAX_ROW) {
        for (uint8_t r = 0; r < MAX_ROW; r++) {
            for (uint8_t c = 0; c < MAX_COL; c++) {
                nodeSet.push_back(node);
                nodeSet.back().lastPlay[0] = r;
                nodeSet.back().lastPlay[1] = c;

                nodeSet.back().currBoard[r][c] = player; 
            }
        }
        return nodeSet;
    }

    // checking if only one space remains
    uint8_t count = 0; bool onlyOneRemaining = true;
    uint8_t index[2];
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        for (uint8_t c = 0; c < MAX_COL; c++) {
            count += node.currBoard[r][c] == '\0';
            if(node.currBoard[r][c] == '\0') {
                index[0] = r;
                index[1] = c;
            }

            // if number of empty spaces is greater than one, then more than one remaining
            if (count > 1) {
                onlyOneRemaining = false;
                break;
            }
        }
        // break if more than one found
        if (!onlyOneRemaining)
            break;
    }

    if (!onlyOneRemaining) {
        // r + 1, c
        if (node.lastPlay[0] + 1 < MAX_ROW && node.currBoard[node.lastPlay[0] + 1][node.lastPlay[1]] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0] + 1;
            nodeSet.back().lastPlay[1] = node.lastPlay[1];

            nodeSet.back().currBoard[node.lastPlay[0] + 1][node.lastPlay[1]] = player; 
        }
        // r - 1, c
        if (node.lastPlay[0] - 1 >= 0 && node.currBoard[node.lastPlay[0] - 1][node.lastPlay[1]] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0] - 1;
            nodeSet.back().lastPlay[1] = node.lastPlay[1];

            nodeSet.back().currBoard[node.lastPlay[0] - 1][node.lastPlay[1]] = player; 
        }
        // r, c + 1
        if (node.lastPlay[1] + 1 < MAX_COL && node.currBoard[node.lastPlay[0]][node.lastPlay[1] + 1] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0];
            nodeSet.back().lastPlay[1] = node.lastPlay[1] + 1;

            nodeSet.back().currBoard[node.lastPlay[0]][node.lastPlay[1] + 1] = player;
        }
        // r, c - 1
        if (node.lastPlay[1] - 1 >= 0 && node.currBoard[node.lastPlay[0]][node.lastPlay[1] - 1] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0];
            nodeSet.back().lastPlay[1] = node.lastPlay[1] - 1;

            nodeSet.back().currBoard[node.lastPlay[0]][node.lastPlay[1] - 1] = player; 
        }
        // r + 1, c + 1
        if (node.lastPlay[1] + 1 < MAX_COL && node.lastPlay[0] + 1 < MAX_ROW && node.currBoard[node.lastPlay[0] + 1][node.lastPlay[1] + 1] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0] + 1;
            nodeSet.back().lastPlay[1] = node.lastPlay[1] + 1;

            nodeSet.back().currBoard[node.lastPlay[0] + 1][node.lastPlay[1] + 1] = player; 
        }
        // r + 1, c - 1
        if (node.lastPlay[1] - 1 >= 0 && node.lastPlay[0] + 1 < MAX_ROW && node.currBoard[node.lastPlay[0] + 1][node.lastPlay[1] - 1] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0] + 1;
            nodeSet.back().lastPlay[1] = node.lastPlay[1] - 1;

            nodeSet.back().currBoard[node.lastPlay[0] + 1][node.lastPlay[1] - 1] = player; 
        }
        // r - 1, c + 1
        if (node.lastPlay[1] + 1 < MAX_COL && node.lastPlay[0] - 1 >= 0 && node.currBoard[node.lastPlay[0] - 1][node.lastPlay[1] + 1] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0] - 1;
            nodeSet.back().lastPlay[1] = node.lastPlay[1] + 1;

            nodeSet.back().currBoard[node.lastPlay[0] - 1][node.lastPlay[1] + 1] = player; 
        }
        // r - 1, c - 1
        if (node.lastPlay[1] - 1 >= 0 && node.lastPlay[0] - 1 >= 0 && node.currBoard[node.lastPlay[0] - 1][node.lastPlay[1] - 1] == '\0') {
            nodeSet.push_back(node);
            nodeSet.back().lastPlay[0] = node.lastPlay[0] - 1;
            nodeSet.back().lastPlay[1] = node.lastPlay[1] - 1;

            nodeSet.back().currBoard[node.lastPlay[0] - 1][node.lastPlay[1] - 1] = player; 
        }

        // if there is no empty spots adjacent to last play, then all empty spots are available
        if (nodeSet.size() == 0) {
            for (uint8_t r = 0; r < MAX_ROW; r++) {
                for (uint8_t c = 0; c < MAX_COL; c++) {
                    // if empty spot, add to set of possible moves
                    if (node.currBoard[r][c] == '\0') {
                        nodeSet.push_back(node);
                        nodeSet.back().lastPlay[0] = r;
                        nodeSet.back().lastPlay[1] = c;

                        nodeSet.back().currBoard[r][c] = player; // AI is black, person is white
                    }
                }
            }
        }
    }
    else {
        // makes remaining move (one empty spot)
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = index[0];
        nodeSet.back().lastPlay[1] = index[1];

        nodeSet.back().currBoard[index[0]][index[1]] = player; 

        // does not make last move
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = index[0];
        nodeSet.back().lastPlay[1] = index[1];

        nodeSet.back().currBoard[index[0]][index[1]] = ' '; // space == last space

    }
    return nodeSet;
}

// returns whether this is a leaf node - no empty spots available
bool IsLeaf(const Node& node) {
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        for (uint8_t c = 0; c < MAX_COL; c++) {
            if (node.currBoard[r][c] == '\0')
                return false;
        }
    }
    return true;
}

// Checks number of lives for given player
uint8_t NumLives(const Node& node, const char& playChar) { // player = true == AI == 'B'
    uint8_t numLives = 0;
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        for (uint8_t c = 0; c < MAX_COL; c++) {
            if (node.currBoard[r][c] == playChar) {
                // checking North
                if (r + 3 < MAX_ROW) {
                    if (node.currBoard[r + 3][c] == playChar && node.currBoard[r + 1][c] == playChar && node.currBoard[r + 2][c] == playChar &&
                        (r + 4 < MAX_ROW ? node.currBoard[r + 4][c] != playChar : true) && (r - 1 >= 0 ? node.currBoard[r - 1][c] != playChar : true))
                    {
                        numLives++;
                    }
                }

                // checking North-East
                if (c + 3 < MAX_COL && r + 3 < MAX_ROW) {
                    if (node.currBoard[r + 3][c + 3] == playChar && node.currBoard[r + 1][c + 1] == playChar && node.currBoard[r + 2][c + 2] == playChar
                        && (r + 4 < MAX_ROW && c + 4 < MAX_COL ? node.currBoard[r + 4][c + 4] != playChar : true) && (r - 1 >= 0 && c - 1 >= 0 ? node.currBoard[r - 1][c - 1] != playChar : true))
                    {
                        numLives++;
                    }
                }

                // checking East
                if (c + 3 < MAX_COL) {
                    if (node.currBoard[r][c + 3] == playChar && node.currBoard[r][c + 1] == playChar && node.currBoard[r][c + 2] == playChar &&
                        (c + 4 < MAX_COL ? node.currBoard[r][c + 4] != playChar : true) && (c - 1 >= 0 ? node.currBoard[r][c - 1] != playChar : true))
                    {
                        numLives++;

                    }
                }

                // checking Sourth-East
                if (c + 3 < MAX_COL && r - 3 >= 0) {
                    if (node.currBoard[r - 3][c + 3] == playChar && node.currBoard[r - 1][c + 1] == playChar && node.currBoard[r - 2][c + 2] == playChar
                        && (r - 4 >= 0 && c + 4 < MAX_COL ? node.currBoard[r - 4][c + 4] != playChar : true) && (r + 1 < MAX_ROW && c - 1 >= 0 ? node.currBoard[r + 1][c - 1] != playChar : true))
                    {
                        numLives++;
                    }
                }
            }
        }
    }

    return numLives;
}

// returns the node's score
score NodeScore(const Node& node) {
    // counting number of lives
    uint8_t numLives = 0;
    uint8_t numSingles = 0;
    uint8_t numDoubles = 0;
    uint8_t numTriples = 0;
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        for (uint8_t c = 0; c < MAX_COL; c++) {
            if (node.currBoard[r][c] == AI_sym) {
                // checking south
                if (r + 3 < MAX_ROW && (r + 4 < MAX_ROW ? node.currBoard[r + 4][c] != AI_sym : true) && (r - 1 >= 0 ? node.currBoard[r - 1][c] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = r; i <= r + 3; i++) {
                        if (node.currBoard[i][c] == AI_sym)
                            count++;
                        else if (node.currBoard[i][c] != '\0') { // if other player's stone in segment
                            count = -1;
                            break;
                        }
                    }

                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    default:
                        numLives++;
                        break;
                    }
                }

                /// checking north 3
                if (r - 3 >= 0 && node.currBoard[r - 3][c] == '\0' && node.currBoard[r - 1][c] == '\0' && node.currBoard[r - 2][c] == '\0'
                    && (r - 4 >= 0 ? node.currBoard[r - 4][c] != AI_sym : true) && (r + 1 < MAX_ROW ? node.currBoard[r + 1][c] != AI_sym : true)) {
                    numSingles++;
                }

                // checking north 2 and south one
                if (r - 2 >= 0 && r + 1 < MAX_ROW && (r - 3 >= 0 ? node.currBoard[r - 3][c] != AI_sym : true) && (r + 2 < MAX_ROW ? node.currBoard[r + 2][c] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = r - 2; i <= r + 1; i++) {
                        if (node.currBoard[i][c] == AI_sym && i >= r)
                            count++;
                        else if (node.currBoard[i][c] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }

                // checking north 1 and south 2
                if (r - 1 >= 0 && r + 2 < MAX_ROW && (r - 2 >= 0 ? node.currBoard[r - 2][c] != AI_sym : true) && (r + 3 < MAX_ROW ? node.currBoard[r + 3][c] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = r - 1; i <= r + 2; i++) {
                        if (node.currBoard[i][c] == AI_sym && i >= r)
                            count++;
                        else if (node.currBoard[i][c] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }

                /////////////////

                // checking south-East
                if (c + 3 < MAX_COL && r + 3 < MAX_ROW && (r + 4 < MAX_ROW && c + 4 < MAX_COL ? node.currBoard[r + 4][c + 4] != AI_sym : true) && (r - 1 >= 0 && c - 1 >= 0 ? node.currBoard[r - 1][c - 1] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = 1; i <= 3; i++) {
                        if (node.currBoard[r + i][c + i] == AI_sym)
                            count++;
                        else if (node.currBoard[r + i][c + i] != '\0') { // if other player's stone in segment
                            count = -1;
                            break;
                        }
                    }

                    switch (count) {
                    case -1:
                        break;
                    case 0:
                        numSingles++;
                        break;
                    case 1:
                        numDoubles++;
                        break;
                    case 2:
                        numTriples++;
                        break;
                    default:
                        numLives++;
                        break;
                    }
                }

                // checking North-West diagnal
                if (c - 3 >= 0 && r - 3 >= 0 && node.currBoard[r - 3][c - 3] == '\0' && node.currBoard[r - 1][c - 1] == '\0' && node.currBoard[r - 2][c - 2] == '\0'
                    && (r - 4 >= 0 && c - 4 >= 0 ? node.currBoard[r - 4][c - 4] != AI_sym : true) && (r + 1 < MAX_ROW && c + 1 < MAX_COL ? node.currBoard[r + 1][c + 1] != AI_sym : true)) {
                    numSingles++;
                }

                // checking North 2, West 2 and south 1, east 1 diagnal
                if (c - 2 >= 0 && c + 1 < MAX_COL && r - 2 >= 0 && r + 1 < MAX_ROW 
                    && (r - 3 >= 0 && c - 3 >= 0 ? node.currBoard[r - 3][c - 3] != AI_sym : true) && (r + 2 < MAX_ROW && c + 2 < MAX_COL ? node.currBoard[r + 2][c + 2] != AI_sym : true)) {

                    int8_t count = 0;
                    for (int8_t i = -2; i <= 1; i++) {
                        if (node.currBoard[r + i][c + i] == AI_sym && i >= 0)
                            count++;
                        else if (node.currBoard[r + i][c + i] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }

                if (c - 1 >= 0 && c + 2 < MAX_COL && r - 1 >= 0 && r + 2 < MAX_ROW
                    && (r - 2 >= 0 && c - 2 >= 0 ? node.currBoard[r - 2][c - 2] != AI_sym : true) && (r + 3 < MAX_ROW && c + 3 < MAX_COL ? node.currBoard[r + 3][c + 3] != AI_sym : true)) {

                    int8_t count = 0;
                    for (int8_t i = -1; i <= 2; i++) {
                        if (node.currBoard[r + i][c + i] == AI_sym && i >= 0)
                            count++;
                        else if (node.currBoard[r + i][c + i] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }

                /////////////////

                // checking East
                if (c + 3 < MAX_COL && (c + 4 < MAX_COL ? node.currBoard[r][c + 4] != AI_sym : true) && (c - 1 >= 0 ? node.currBoard[r][c - 1] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = c + 1; i <= c + 3; i++) {
                        if (node.currBoard[r][i] == AI_sym)
                            count++;
                        else if (node.currBoard[r][i] != '\0') { // if other player's stone in segment
                            count = -1;
                            break;
                        }
                    }

                    switch (count) {
                    case -1:
                        break;
                    case 0:
                        numSingles++;
                        break;
                    case 1:
                        numDoubles++;
                        break;
                    case 2:
                        numTriples++;
                        break;
                    default:
                        numLives++;
                        break;
                    }
                }

                /// checking west 3
                if (c - 3 >= 0 && node.currBoard[r][c - 3] == '\0' && node.currBoard[r][c - 2] == '\0' && node.currBoard[r][c - 1] == '\0'
                    && (c - 4 >= 0 ? node.currBoard[r][c - 4] != AI_sym : true) && (c + 1 < MAX_COL ? node.currBoard[r][c + 1] != AI_sym : true)) {
                    numSingles++;
                }

                // checking west 2 and east 1
                if (c - 2 >= 0 && c + 1 < MAX_COL && (c - 3 >= 0 ? node.currBoard[r][c - 3] != AI_sym : true) && (c + 2 < MAX_COL ? node.currBoard[r][c + 2] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = c - 2; i <= c + 1; i++) {
                        if (node.currBoard[r][i] == AI_sym && i >= c)
                            count++;
                        else if (node.currBoard[r][i] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }

                // checking west 1 and east 2
                if (c - 1 >= 0 && c + 2 < MAX_COL && (c - 2 >= 0 ? node.currBoard[r][c - 2] != AI_sym : true) && (c + 3 < MAX_COL ? node.currBoard[r][c + 3] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = c - 1; i <= c + 2; i++) {
                        if (node.currBoard[r][i] == AI_sym && i >= c)
                            count++;
                        else if (node.currBoard[r][i] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }

                /////////////////

                // checking South-west
                if (c - 3 >= 0 && r + 3 < MAX_ROW && (r + 4 < MAX_ROW && c - 4 >= 0 ? node.currBoard[r + 4][c - 4] != AI_sym : true) && (r - 1 >= 0 && c + 1 < MAX_COL ? node.currBoard[r - 1][c + 1] != AI_sym : true)) {
                    int8_t count = 0;
                    for (uint8_t i = 1; i <= 3; i++) {
                        if (node.currBoard[r + i][c - i] == AI_sym)
                            count++;
                        else if (node.currBoard[r + i][c - i] != '\0') { // if other player's stone in segment
                            count = -1;
                            break;
                        }
                    }

                    switch (count) {
                    case -1:
                        break;
                    case 0:
                        numSingles++;
                        break;
                    case 1:
                        numDoubles++;
                        break;
                    case 2:
                        numTriples++;
                        break;
                    default:
                        numLives++;
                        break;
                    }
                }

                // checking north-east diagnal
                if (c + 3 < MAX_COL && r - 3 >= 0 && node.currBoard[r - 3][c + 3] == '\0' && node.currBoard[r - 1][c + 1] == '\0' && node.currBoard[r - 2][c + 2] == '\0'
                    && (r - 4 >= 0 && c + 4 < MAX_COL ? node.currBoard[r - 4][c + 4] != AI_sym : true) && (r + 1 < MAX_ROW && c - 1 >= 0 ? node.currBoard[r + 1][c - 1] != AI_sym : true)) {
                    numSingles++;
                }

                if (c + 2 < MAX_COL && c - 1 >= 0 && r + 1 < MAX_ROW && r - 2 >=0 
                    && (r - 3 >= 0 && c + 3 < MAX_COL ? node.currBoard[r - 3][c + 3] != AI_sym : true) && (r + 2 < MAX_ROW && c - 2 >= 0 ? node.currBoard[r + 2][c - 2] != AI_sym : true)) {

                    int8_t count = 0;
                    for (int8_t i = -2; i <= 1; i++) {
                        if (node.currBoard[r + i][c - i] == AI_sym && i >= 0)
                            count++;
                        else if (node.currBoard[r + i][c - i] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }

                if (c + 1 < MAX_COL && c - 2 >= 0 && r + 2 < MAX_ROW && r - 1 >= 0
                    && (r - 2 >= 0 && c + 2 < MAX_COL ? node.currBoard[r - 2][c + 2] != AI_sym : true) && (r + 3 < MAX_ROW && c - 3 >= 0 ? node.currBoard[r + 3][c - 3] != AI_sym : true)) {

                    int8_t count = 0;
                    for (int8_t i = -1; i <= 2; i++) {
                        if (node.currBoard[r + i][c - i] == AI_sym && i >= 0)
                            count++;
                        else if (node.currBoard[r + i][c - i] != '\0') { // not player stone found
                            count = -1;
                            break;
                        }
                    }
                    switch (count) {
                    case -1:
                        break;
                    case 1:
                        numSingles++;
                        break;
                    case 2:
                        numDoubles++;
                        break;
                    case 3:
                        numTriples++;
                        break;
                    }
                }
            }
        }
    }

    // for each peice combined I add one
    return numSingles + numDoubles*2 + numTriples*3 + numLives*4;
}

// progressive minimax algorithm variables
atomic<bool> exit_thread_flag;
unsigned int MAX_DEPTH; Node next_move; bool LEAF;

// minimax search algorithm
score Minimax(Node node, unsigned int depth, bool isMax, score alpha, score beta) {
    // if thread has been flagged to stop minimax, return -1
    if (exit_thread_flag)
        return -1;
    
    // if node is leaf, then flag as leaf node and return score
    if (IsLeaf(node)) {
        LEAF = true;
        return NodeScore(node);
    }

    // if max depth, return score
    if (depth == MAX_DEPTH)
        return NodeScore(node);
    
    score bestVal;
    if (isMax) {
        bestVal = MIN_SCORE;
        for (auto& child : PossibleMoves(node, AI_sym)) {
            score value = Minimax(child, depth + 1, false, alpha, beta);
            if (value == -1)
                return -1;
            else if (depth == 0) {
                score preBest = bestVal;
                bestVal = Max(bestVal, value);
                if (bestVal > preBest) {
                    for (uint8_t r = 0; r < MAX_ROW; r++) {
                        for (uint8_t c = 0; c < MAX_COL; c++) {
                            next_move.currBoard[r][c] = child.currBoard[r][c];
                        }
                    }
                    next_move.lastPlay[0] = child.lastPlay[0];
                    next_move.lastPlay[1] = child.lastPlay[1];
                }
            }
            else {
                bestVal = Max(bestVal, value);
            }
            alpha = Max(alpha, bestVal);
            if (beta <= alpha) {
                break;
            }
        }
    }
    else {
        bestVal = MAX_SCORE;
        for (auto& child : PossibleMoves(node, Human_sym)) {
            score value = Minimax(child, depth + 1, true, alpha, beta);
            if (value == -1)
                return -1;
            else if (depth == 0) {
                score preBest = bestVal;
                bestVal = Min(bestVal, value);
                if (bestVal < preBest)
                {
                    for (uint8_t r = 0; r < MAX_ROW; r++) {
                        for (uint8_t c = 0; c < MAX_COL; c++) {
                            next_move.currBoard[r][c] = child.currBoard[r][c];
                        }
                    }
                    next_move.lastPlay[0] = child.lastPlay[0];
                    next_move.lastPlay[1] = child.lastPlay[1];
                }
            }
            else {
                bestVal = Min(bestVal, value);
            }
            beta = Min(beta, bestVal);
            if (beta <= alpha) {
                break;
            }
        }
    }
    return bestVal;
}

void RunMinimax(const Node& node, Node& next_node, bool& leafFound, bool player) {
    LEAF = false; MAX_DEPTH = 1;

    // loop while minimax has not returned leaf (reached bottom of tree) and exit has not been flagged
    while (!LEAF && !exit_thread_flag) {
        // run minimax with specified depth
        if (Minimax(node, 0, player, MIN_SCORE, MAX_SCORE) != -1) { // if thread did not recv exit flag, then copy data
            for (uint8_t r = 0; r < MAX_ROW; r++) {
                for (uint8_t c = 0; c < MAX_COL; c++) {
                    next_node.currBoard[r][c] = next_move.currBoard[r][c];
                }
            }
            next_node.lastPlay[0] = next_move.lastPlay[0];
            next_node.lastPlay[1] = next_move.lastPlay[1];
        }
        MAX_DEPTH++; // increasing depth

        leafFound = LEAF;
    }
}

// diplays game board
// http://web.theurbanpenguin.com/adding-color-to-your-output-from-c/
void PrintBoard(const Node& node) {

    printf("\n");
    char letter = 'A';
    printf("  ");
    for (uint8_t r = 1; r <= MAX_ROW; r++) {
        printf(" %i |", r);
    }
    cout << endl;
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        cout << " " << string(2 + 4* MAX_COL, '-') << endl;
        printf("%c|", letter++);
        for (uint8_t c = 0; c < MAX_COL; c++) {
            if (r == node.lastPlay[0] && c == node.lastPlay[1]) {
                printf("\033[1;32m");
                printf(" %c ", node.currBoard[r][c]);
                printf("\033[0m");
            }
            else if (node.currBoard[r][c] == 'B') {
                printf("\033[1;34m");
                printf(" %c ", node.currBoard[r][c]);
                printf("\033[0m");
            }
            else if (node.currBoard[r][c] == 'W') {
                //printf("\033[1;32m");
                printf(" %c ", node.currBoard[r][c]);
                //printf("\033[0m");
            }
            else {
                printf("   ");
            }
            printf("|");
        }
        cout << endl;
    }
    cout << " " << string(2 + 4 * MAX_COL, '-') << endl;
}

// user input checking
int8_t LetterToRow(const char& letter) {
    if (letter <= 'z' && letter >= 'a') {
        return letter - 97;
    }
    else if (letter <= 'Z' && letter >= 'A') {
        return letter - 65;
    }
    else {
        printf("ERROR: Please re-enter\n\a");

        // Clearing and ignoring the last cin
        cin.clear();
        cin.ignore();

        return -1;
    }
}
bool ValidMove(const Node& node, const int8_t& r, const int8_t& c) {
    // if initial move
    if (node.lastPlay[0] == MAX_ROW)
        return true;

    if (node.currBoard[r][c] == '\0') {
        int count = 0;
        // checking for adjecency
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1;j++) {
                if (r == (node.lastPlay[0] + i) && c == (node.lastPlay[1] + j))
                    return true;
                else if (node.currBoard[node.lastPlay[0] + i][node.lastPlay[1] + j] != '\0')
                    count++;
            }

            // if all adjecent spaces are filled (i.e., choose random spot)
            if (count == 8)
                return true;
        }
    }
    return false;
}

int main() {
    // Changing the color of the exe window
    cout << "\tPLEASE HAVE AUDIO ON!!! There is Music Playing in the background. \n\n";
    cout << "\n\nWelcome to Freedom!\n";

    bool valid = false;
    do {
        char stone;
        cout << "\nDo you want to be White or Black Stone (W/B):";
        cin >> stone;
        if (stone == 'B' || stone == 'b') {
            Human_sym = 'B';
            AI_sym = 'W';
            valid = true;
        }
        else if (stone == 'w' || stone == 'W') {
            Human_sym = 'W';
            AI_sym = 'B';
            valid = true;
        }
        else {
            printf("ERROR: Please re-enter\n\a");

            // Clearing and ignoring the last cin
            cin.clear();
            cin.ignore();
        }
    } while (!valid);

    // Clearing any music that might be playing in the back ground
    Clear();

    // Starting the back gound music
    BattleMusic();

    // declare node and print board
    Node node;
    PrintBoard(node);

    bool turn = Human_sym == 'W';   // true == human turn, false == ai turn
    
    while (!IsLeaf(node)) {
        if (turn) {
            uint8_t count = 0; bool onlyOneRemaining = true;
            uint8_t index[2];
            for (uint8_t r = 0; r < MAX_ROW; r++) {
                for (uint8_t c = 0; c < MAX_COL; c++) {
                    count += node.currBoard[r][c] == '\0';
                    if (node.currBoard[r][c] == '\0') {
                        index[0] = r;
                        index[1] = c;
                    }
                    if (count > 1) {
                        onlyOneRemaining = false;
                        break;
                    }
                }
            }
            if (onlyOneRemaining) {
                bool valid;
                do {
                    valid = true;
                    char noYes;
                    printf("Only one space remaining. Would you like to leave empty? (Y/N): ");
                    cin >> noYes;

                    if (noYes == 'n' || noYes == 'N') {
                        node.lastPlay[0] = index[0];
                        node.lastPlay[1] = index[1];
                        node.currBoard[index[0]][index[1]] = ' ';
                    }
                    else if (noYes == 'y' || noYes == 'Y') {
                        node.lastPlay[0] = index[0];
                        node.lastPlay[1] = index[1];
                        node.currBoard[index[0]][index[1]] = 'W';
                    }
                    else {
                        printf("ERROR: Please re-enter\n\a");

                        // Clearing and ignoring the last cin
                        cin.clear();
                        cin.ignore();
                    }

                } while (!valid);
            }
            else {
                char letter; int col; int8_t row;
                do {
                    printf("Please make a move w/%c stone (RowCol Ex: A3): ", Human_sym);
                    cin >> letter >> col;
                    col--;
                    row = LetterToRow(letter);
                    if (row != -1) {
                        if (!ValidMove(node, row, col)) {
                            printf("ERROR: Please re-enter\n\a");
                            row = -1;

                            // Clearing and ignoring the last cin
                            cin.clear();
                            cin.ignore();
                        }
                    }
                } while (row == -1);
                node.lastPlay[0] = row;
                node.lastPlay[1] = col;
                node.currBoard[row][col] = Human_sym;
            }

        }
        else {
            // progressive minimax (allows for variable depth approximations)
            exit_thread_flag = false;
            Node nextNode;
            bool LeafFound = false;
            thread minimaxThread(&RunMinimax, ref(node), ref(nextNode), ref(LeafFound), true);
            int interval = 0;
            
            // max computational time is 5-seconds
            while (!LeafFound && interval < 5) {
                this_thread::sleep_for(1s);
                interval++;
            }
            
            // signal to stop processing and join main thread
            exit_thread_flag = true;
            minimaxThread.join();

            // setting AI's move
            node.currBoard[nextNode.lastPlay[0]][nextNode.lastPlay[1]] = nextNode.currBoard[nextNode.lastPlay[0]][nextNode.lastPlay[1]];
            node.lastPlay[0] = nextNode.lastPlay[0];
            node.lastPlay[1] = nextNode.lastPlay[1];
        }
        turn = !turn;
        PrintBoard(node);
    }
    // checking who has more lives
    uint8_t person = NumLives(node, Human_sym);
    uint8_t AI = NumLives(node, AI_sym);
    if (person > AI) {
        Clear(); // clearing the last song
        Victory(); // Play victory song for user
        cout << "\tPerson Won! GG! \n\n";
    }
    else if (AI > person) {
        Clear(); // clearing the last song
        CompWin(); // Play computer win song
        cout << "\tComputer Won! GG!\n\n";
    }
    else {
        // Stop the current song
        Clear();

        // start playing Cat song
        CatVictory();
        cout << "\tThe CAT Won! DRAW GG!\n\n";
    }

    system("pause");
    return 0;
}