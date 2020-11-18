#include <vector>
#include <string>
#include <iostream>
#include "AudioPlayer.h"
#define MAX_ROW 8
#define MAX_COL 8
#define MAX_SCORE INT_MAX
#define MIN_SCORE INT_MIN
typedef unsigned int score;
using namespace std;

// node represents game board
struct Node {
    uint8_t lastPlay[2];    // last play to create current board. row, col
    char currBoard[MAX_ROW][MAX_COL];
    Node() {
        lastPlay[0] = -1;
        lastPlay[1] = -1;
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
vector<Node> PossibleMoves(const Node& node, bool player) {
    vector<Node> nodeSet;

    // r + 1, c
    if (node.lastPlay[0] + 1 < MAX_ROW && node.currBoard[node.lastPlay[0] + 1][node.lastPlay[1]] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0] + 1;
        nodeSet.back().lastPlay[1] = node.lastPlay[1];

        nodeSet.back().currBoard[node.lastPlay[0] + 1][node.lastPlay[1]] = 'B' * player + 'W' * !player; // AI is black, person is white
    }
    // r - 1, c
    if (node.lastPlay[0] - 1 >= 0 && node.currBoard[node.lastPlay[0] - 1][node.lastPlay[1]] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0] - 1;
        nodeSet.back().lastPlay[1] = node.lastPlay[1];

        nodeSet.back().currBoard[node.lastPlay[0] - 1][node.lastPlay[1]] = 'B' * player + 'W' * !player; // AI is black, person is white
    }
    // r, c + 1
    if (node.lastPlay[1] + 1 < MAX_COL && node.currBoard[node.lastPlay[0]][node.lastPlay[1] + 1] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0];
        nodeSet.back().lastPlay[1] = node.lastPlay[1] + 1;

        nodeSet.back().currBoard[node.lastPlay[0]][node.lastPlay[1] + 1] = 'B' * player + 'W' * !player; // AI is black, person is white
    }
    // r, c - 1
    if (node.lastPlay[1] - 1 >= 0 && node.currBoard[node.lastPlay[0]][node.lastPlay[1] - 1] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0];
        nodeSet.back().lastPlay[1] = node.lastPlay[1] - 1;

        nodeSet.back().currBoard[node.lastPlay[0]][node.lastPlay[1] - 1] = 'B' * player + 'W' * !player; // AI is black, person is white
    }
    // r + 1, c + 1
    if (node.lastPlay[1] + 1 < MAX_COL && node.lastPlay[0] + 1 < MAX_ROW && node.currBoard[node.lastPlay[0] + 1][node.lastPlay[1] + 1] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0] + 1;
        nodeSet.back().lastPlay[1] = node.lastPlay[1] + 1;

        nodeSet.back().currBoard[node.lastPlay[0] + 1][node.lastPlay[1] + 1] = 'B' * player + 'W' * !player; // AI is black, person is white
    }
    // r + 1, c - 1
    if (node.lastPlay[1] - 1 >= 0 && node.lastPlay[0] + 1 < MAX_ROW && node.currBoard[node.lastPlay[0] + 1][node.lastPlay[1] - 1] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0] + 1;
        nodeSet.back().lastPlay[1] = node.lastPlay[1] - 1;

        nodeSet.back().currBoard[node.lastPlay[0] + 1][node.lastPlay[1] - 1] = 'B' * player + 'W' * !player; // AI is black, person is white
    }
    // r - 1, c + 1
    if (node.lastPlay[1] + 1 < MAX_COL && node.lastPlay[0] - 1 >= 0 && node.currBoard[node.lastPlay[0] - 1][node.lastPlay[1] + 1] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0] - 1;
        nodeSet.back().lastPlay[1] = node.lastPlay[1] + 1;

        nodeSet.back().currBoard[node.lastPlay[0] - 1][node.lastPlay[1] + 1] = 'B' * player + 'W' * !player; // AI is black, person is white
    }
    // r - 1, c - 1
    if (node.lastPlay[1] - 1 >= 0 && node.lastPlay[0] - 1 >= 0 && node.currBoard[node.lastPlay[0] - 1][node.lastPlay[1] - 1] == '\0') {
        nodeSet.push_back(node);
        nodeSet.back().lastPlay[0] = node.lastPlay[0] - 1;
        nodeSet.back().lastPlay[1] = node.lastPlay[1] - 1;

        nodeSet.back().currBoard[node.lastPlay[0] - 1][node.lastPlay[1] - 1] = 'B' * player + 'W' * !player; // AI is black, person is white
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

                    nodeSet.back().currBoard[r][c] = 'B' * player + 'W' * !player; // AI is black, person is white
                }
            }
        }
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
uint8_t NumLives(const Node& node, char playChar) { // player = true == AI == 'B'
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
    char playChar = 'B';

    // counting number of lives
    uint8_t numLives = 0;
    uint8_t numSingles = 0;
    uint8_t numDoubles = 0;
    uint8_t numTriples = 0;
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
                    // if a stone has a path to get live (i.e., empty spaces)
                    else if (node.currBoard[r + 3][c] == '\0' && node.currBoard[r + 1][c] == '\0' && node.currBoard[r + 2][c] == '\0' &&
                        (r + 4 < MAX_ROW ? node.currBoard[r + 4][c] != playChar : true) && (r - 1 >= 0 ? node.currBoard[r - 1][c] != playChar : true)) 
                    {
                        numSingles++;
                    }
                    // if three stones have a path to get a live 
                    else if (((node.currBoard[r + 3][c] == playChar && (node.currBoard[r + 1][c] == playChar || node.currBoard[r + 2][c] == playChar) && (node.currBoard[r + 1][c] == '\0' || node.currBoard[r + 2][c] == '\0'))
                        || (node.currBoard[r + 2][c] == playChar && (node.currBoard[r + 1][c] == playChar || node.currBoard[r + 3][c] == playChar) && (node.currBoard[r + 1][c] == '\0' || node.currBoard[r + 3][c] == '\0'))
                        || (node.currBoard[r + 1][c] == playChar && (node.currBoard[r + 2][c] == playChar || node.currBoard[r + 3][c] == playChar) && (node.currBoard[r + 2][c] == '\0' || node.currBoard[r + 3][c] == '\0')))
                        && (r + 4 < MAX_ROW ? node.currBoard[r + 4][c] != playChar : true) && (r - 1 >= 0 ? node.currBoard[r - 1][c] != playChar : true))
                    {
                        numTriples++;
                    }
                    // if two stones have path to get a live
                    else if (((node.currBoard[r + 3][c] == playChar && node.currBoard[r + 1][c] == '\0' && node.currBoard[r + 2][c] == '\0')
                        || (node.currBoard[r + 1][c] == playChar && node.currBoard[r + 2][c] == '\0' && node.currBoard[r + 3][c] == '\0')
                        || (node.currBoard[r + 2][c] == playChar && node.currBoard[r + 1][c] == '\0' && node.currBoard[r + 3][c] == '\0')) 
                        && (r + 4 < MAX_ROW ? node.currBoard[r + 4][c] != playChar : true) && (r - 1 >= 0 ? node.currBoard[r - 1][c] != playChar : true)) 
                    {
                        numDoubles++;
                    }
                }

                // checking North-East
                if (c + 3 < MAX_COL && r + 3 < MAX_ROW) {
                    if (node.currBoard[r + 3][c + 3] == playChar && node.currBoard[r + 1][c + 1] == playChar && node.currBoard[r + 2][c + 2] == playChar
                        && (r + 4 < MAX_ROW && c + 4 < MAX_COL ? node.currBoard[r + 4][c + 4] != playChar : true) && (r - 1 >= 0 && c - 1 >= 0 ? node.currBoard[r - 1][c - 1] != playChar : true))
                    {
                        numLives++;
                    }
                    // if three stones have a path to get a live 
                    else if (node.currBoard[r + 3][c + 3] == '\0' && node.currBoard[r + 1][c + 1] == '\0' && node.currBoard[r + 2][c + 2] == '\0' &&
                        (r + 4 < MAX_ROW && c + 4 < MAX_COL ? node.currBoard[r + 4][c + 4] != playChar : true) && (r - 1 >= 0 && c - 1 >= 0 ? node.currBoard[r - 1][c - 1] != playChar : true))
                    {
                        numSingles++;
                    }
                    // if three stones have a path to get a live 
                    else if (((node.currBoard[r + 3][c + 3] == playChar && (node.currBoard[r + 1][c + 1] == playChar || node.currBoard[r + 2][c + 2] == playChar) && (node.currBoard[r + 1][c + 1] == '\0' || node.currBoard[r + 2][c + 2] == '\0'))
                        || (node.currBoard[r + 2][c + 2] == playChar && (node.currBoard[r + 1][c + 1] == playChar || node.currBoard[r + 3][c + 3] == playChar) && (node.currBoard[r + 1][c + 1] == '\0' || node.currBoard[r + 3][c + 3] == '\0'))
                        || (node.currBoard[r + 1][c + 1] == playChar && (node.currBoard[r + 2][c + 2] == playChar || node.currBoard[r + 3][c + 3] == playChar) && (node.currBoard[r + 2][c + 2] == '\0' || node.currBoard[r + 3][c + 3] == '\0')))
                        && (r + 4 < MAX_ROW && c + 4 < MAX_COL ? node.currBoard[r + 4][c + 4] != playChar : true) && (r - 1 >= 0 && c - 1 >= 0 ? node.currBoard[r - 1][c - 1] != playChar : true))
                    {
                        numTriples++;
                    }
                    // if two stones have path to get a live
                    else if (((node.currBoard[r + 3][c + 3] == playChar && node.currBoard[r + 1][c + 1] == '\0' && node.currBoard[r + 2][c + 2] == '\0')
                        || (node.currBoard[r + 1][c + 1] == playChar && node.currBoard[r + 2][c + 2] == '\0' && node.currBoard[r + 3][c + 3] == '\0')
                        || (node.currBoard[r + 2][c + 2] == playChar && node.currBoard[r + 1][c + 1] == '\0' && node.currBoard[r + 3][c + 3] == '\0'))
                        && (r + 4 < MAX_ROW && c + 4 < MAX_COL ? node.currBoard[r + 4][c + 4] != playChar : true) && (r - 1 >= 0 && c - 1 >= 0 ? node.currBoard[r - 1][c - 1] != playChar : true))
                    {
                        numDoubles++;
                    }
                }

                // checking East
                if (c + 3 < MAX_COL) {
                    if (node.currBoard[r][c + 3] == playChar && node.currBoard[r][c + 1] == playChar && node.currBoard[r][c + 2] == playChar &&
                        (c + 4 < MAX_COL ? node.currBoard[r][c + 4] != playChar : true) && (c - 1 >= 0 ? node.currBoard[r][c - 1] != playChar : true))
                    {
                        numLives++;

                    }
                    else if (node.currBoard[r][c + 3] == '\0' && node.currBoard[r][c + 2] == '\0' && node.currBoard[r][c + 1] == '\0' &&
                        (c + 4 < MAX_COL ? node.currBoard[r][c + 4] != playChar : true) && (c - 1 >= 0 ? node.currBoard[r][c - 1] != playChar : true))
                    {
                        numSingles++;
                    }
                    else if (((node.currBoard[r][c + 3] == playChar && (node.currBoard[r][c + 2] == playChar || node.currBoard[r][c + 1] == playChar) && (node.currBoard[r][c + 2] == '\0' || node.currBoard[r][c + 1] == '\0'))
                        || (node.currBoard[r][c + 2] == playChar && (node.currBoard[r][c + 3] == playChar || node.currBoard[r][c + 1] == playChar) && (node.currBoard[r][c + 3] == '\0' || node.currBoard[r][c + 1] == '\0'))
                        || (node.currBoard[r][c + 1] == playChar && (node.currBoard[r][c + 3] == playChar || node.currBoard[r][c + 2] == playChar) && (node.currBoard[r][c + 3] == '\0' || node.currBoard[r][c + 2] == '\0')))
                        && (c + 4 < MAX_COL ? node.currBoard[r][c + 4] != playChar : true) && (c - 1 >= 0 ? node.currBoard[r][c - 1] != playChar : true))
                    {
                        numTriples++;
                    }
                    else if (((node.currBoard[r][c + 3] == playChar && node.currBoard[r][c + 2] == '\0' && node.currBoard[r][c + 1] == '\0')
                        || (node.currBoard[r][c + 2] == playChar && node.currBoard[r][c + 3] == '\0' && node.currBoard[r][c + 1] == '\0')
                        || (node.currBoard[r][c + 1] == playChar && node.currBoard[r][c + 3] == '\0' && node.currBoard[r][c + 2] == '\0'))
                        && (c + 4 < MAX_COL ? node.currBoard[r][c + 4] != playChar : true) && (c - 1 >= 0 ? node.currBoard[r][c - 1] != playChar : true))
                    {
                        numDoubles++;
                    }
                }

                // checking Sourth-East
                if (c + 3 < MAX_COL && r - 3 >= 0) {
                    if (node.currBoard[r - 3][c + 3] == playChar && node.currBoard[r - 1][c + 1] == playChar && node.currBoard[r - 2][c + 2] == playChar
                        && (r - 4 >= 0 && c + 4 < MAX_COL ? node.currBoard[r - 4][c + 4] != playChar : true) && (r + 1 < MAX_ROW && c - 1 >= 0 ? node.currBoard[r + 1][c - 1] != playChar : true))
                    {
                        numLives++;
                    }
                    // if three stones have a path to get a live 
                    else if (node.currBoard[r - 3][c + 3] == '\0' && node.currBoard[r - 1][c + 1] == '\0' && node.currBoard[r - 2][c + 2] == '\0' &&
                        (r - 4 >= 0 && c + 4 < MAX_COL ? node.currBoard[r - 4][c + 4] != playChar : true) && (r + 1 < MAX_ROW && c - 1 >= 0 ? node.currBoard[r + 1][c - 1] != playChar : true))
                    {
                        numSingles++;
                    }
                    // if three stones have a path to get a live 
                    else if (((node.currBoard[r - 3][c + 3] == playChar && (node.currBoard[r - 1][c + 1] == playChar || node.currBoard[r - 2][c + 2] == playChar) && (node.currBoard[r - 1][c + 1] == '\0' || node.currBoard[r - 2][c + 2] == '\0'))
                        || (node.currBoard[r - 2][c + 2] == playChar && (node.currBoard[r - 1][c + 1] == playChar || node.currBoard[r - 3][c + 3] == playChar) && (node.currBoard[r - 1][c + 1] == '\0' || node.currBoard[r - 3][c + 3] == '\0'))
                        || (node.currBoard[r - 1][c + 1] == playChar && (node.currBoard[r - 2][c + 2] == playChar || node.currBoard[r - 3][c + 3] == playChar) && (node.currBoard[r - 2][c + 2] == '\0' || node.currBoard[r - 3][c + 3] == '\0')))
                        && (r - 4 >= 0 && c + 4 < MAX_COL ? node.currBoard[r - 4][c + 4] != playChar : true) && (r + 1 < MAX_ROW && c - 1 >= 0 ? node.currBoard[r + 1][c - 1] != playChar : true))
                    {
                        numTriples++;
                    }
                    // if two stones have path to get a live
                    else if (((node.currBoard[r - 3][c + 3] == playChar && node.currBoard[r - 1][c + 1] == '\0' && node.currBoard[r - 2][c + 2] == '\0')
                        || (node.currBoard[r - 1][c + 1] == playChar && node.currBoard[r - 2][c + 2] == '\0' && node.currBoard[r - 3][c + 3] == '\0')
                        || (node.currBoard[r - 2][c + 2] == playChar && node.currBoard[r - 1][c + 1] == '\0' && node.currBoard[r - 3][c + 3] == '\0'))
                        && (r - 4 >= 0 && c + 4 < MAX_COL ? node.currBoard[r - 4][c + 4] != playChar : true) && (r + 1 < MAX_ROW && c - 1 >= 0 ? node.currBoard[r + 1][c - 1] != playChar : true))
                    {
                        numDoubles++;
                    }
                }
            }
        }
    }

    // for each peice combined I add one
    return numSingles + numDoubles*2 + numTriples*3 + numLives*4;
}

unsigned int MAX_DEPTH = 4; Node next_move;
// minimax search algorithm
score Minimax(Node node, unsigned int depth, bool isMax, score alpha, score beta) {
    if (depth == MAX_DEPTH || IsLeaf(node))
        return NodeScore(node);

    score bestVal;
    if (isMax) {
        bestVal = MIN_SCORE;
        for (auto& child : PossibleMoves(node, isMax)) {
            score value = Minimax(child, depth + 1, false, alpha, beta);
            if (depth == 0) {
                int preBest = bestVal;
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
        for (auto& child : PossibleMoves(node, isMax)) {
            score value = Minimax(child, depth + 1, true, alpha, beta);
            if (depth == 0) {
                int preBest = bestVal;
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
            alpha = Min(beta, bestVal);
            if (beta <= alpha) {
                break;
            }
        }
    }
    return bestVal;
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
            if (node.currBoard[r][c] == 'B') {
                printf("\033[1;31m");
                printf(" %c ", node.currBoard[r][c]);
                printf("\033[0m");
            }
            else if (node.currBoard[r][c] == 'W') {
                printf("\033[1;32m");
                printf(" %c ", node.currBoard[r][c]);
                printf("\033[0m");
            }
            else {
                printf(" %c ", node.currBoard[r][c]);
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
    return node.currBoard[r][c] == '\0';
}

int main() {
    // Changing the color of the exe window
    cout << "\tPLEASE HAVE AUDIO ON!!! There is Music Playing in the background. \n\n";
    cout << "\n\nWelcome to Freedom!\n";

    // Clearing any music that might be playing in the back ground
    Clear();

    // Starting the back gound music
    BattleMusic();

    // declare node and print board
    Node node;
    PrintBoard(node);

    bool turn = true;   // true == person, false == computer
    bool over = false;
    while (!over) {
        if (turn) {
            char letter; int col; int8_t row;
            do {
                printf("Please make a move w/white stone (RowCol Ex: A3): ");
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
            node.currBoard[row][col] = 'W';
        }
        else {
            Minimax(node, 0, true, MIN_SCORE, MAX_SCORE);

            // setting AI's move
            node.currBoard[next_move.lastPlay[0]][next_move.lastPlay[1]] = next_move.currBoard[next_move.lastPlay[0]][next_move.lastPlay[1]];
            node.lastPlay[0] = next_move.lastPlay[0];
            node.lastPlay[1] = next_move.lastPlay[1];
        }

        over = IsLeaf(node);
        turn = !turn;
        PrintBoard(node);
    }
    uint8_t person = NumLives(node, 'W');
    uint8_t AI = NumLives(node, 'B');
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
    return 0;
}