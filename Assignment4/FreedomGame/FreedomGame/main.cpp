#include <vector>
#include <string>
#include <iostream>
#define MAX_ROW 8
#define MAX_COL 8
#define MAX_SCORE INT_MAX
#define MIN_SCORE INT_MIN
typedef int score;
using namespace std;

// node represents game board
struct Node {
    uint8_t lastPlay[2];    // last play to create current board. row, col
    char currBoard[MAX_ROW][MAX_COL];
    Node() {
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
    return nodeSet;
}

// returns whether this is a leaf node
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
uint8_t NumLives(const Node& node, bool player) { // player = true == AI == 'B'
    char playChar = (player ? 'B' : 'W');
    vector<uint8_t[4][2]> lives;
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        for (uint8_t c = 0; c < MAX_COL; c++) {
            if (node.currBoard[r][c] == playChar) {
                // checking North
                if (r + 3 < MAX_ROW) {
                    if (node.currBoard[r + 3][c] == playChar && node.currBoard[r + 1][c] == playChar && node.currBoard[r + 2][c] == playChar)
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or col then no lives which make this one. 
                            if (l[0][0] - l[1][0] != 0 || l[0][1] != c)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][0] == r || l[i][0] == r + 1 || l[i][0] == r + 2 || l[i][0] == r + 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r + i;
                                live[i][1] = c;
                            }
                            lives.push_back(live);
                        }
                    }
                }

                // checking North-East
                if (c + 3 < MAX_COL && r + 3 < MAX_ROW) {
                    if (node.currBoard[r + 3][c + 3] == playChar && node.currBoard[r + 1][c + 1] == playChar && node.currBoard[r + 2][c + 2] == playChar)
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or col then no lives which make this one. 
                            if ((l[0][1] - l[1][1]) / (l[0][0] - l[1][0]) != 1)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][0] == r || l[i][1] == c || l[i][0] == r + 1 || l[i][1] == c + 1 ||
                                    l[i][0] == r + 2 || l[i][1] == c + 2 || l[i][0] == r + 3 || l[i][1] == c + 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r + i;
                                live[i][1] = c + i;
                            }
                            lives.push_back(live);
                        }
                    }
                }

                // checking East
                if (c + 3 < MAX_COL) {
                    if (node.currBoard[r][c + 3] == playChar && node.currBoard[r][c + 1] == playChar && node.currBoard[r][c + 1] == playChar)
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or r then no lives which make this one. 
                            if ((l[0][1] - l[1][1]) / (l[0][0] - l[1][0]) != 0 || l[0][0] != r)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][1] == c || l[i][1] == c + 1 || l[i][1] == c + 2 || l[i][1] == c + 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r;
                                live[i][1] = c + i;
                            }
                            lives.push_back(live);
                        }
                    }
                }

                // checking Sourth-East
                if (c + 3 < MAX_COL && r - 3 >= 0) {
                    if (node.currBoard[r - 3][c + 3] == playChar && node.currBoard[r - 1][c + 1] == playChar && node.currBoard[r - 2][c + 2] == playChar)
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or col then no lives which make this one. 
                            if ((l[0][1] - l[1][1]) / (l[0][0] - l[1][0]) != -1)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][0] == r || l[i][1] == c || l[i][0] == r - 1 || l[i][1] == c + 1 ||
                                    l[i][0] == r - 2 || l[i][1] == c + 2 || l[i][0] == r - 3 || l[i][1] == c + 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r - i;
                                live[i][1] = c + i;
                            }
                            lives.push_back(live);
                        }
                    }
                }

                // checking South
                if (r - 3 >= 0) {
                    if (node.currBoard[r - 3][c] == 'B' && node.currBoard[r - 1][c] == 'B' && node.currBoard[r - 2][c] == 'B')
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or col then no lives which make this one. 
                            if (l[0][0] - l[1][0] != 0 || l[0][1] != c)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][0] == r || l[i][0] == r - 1 || l[i][0] == r - 2 || l[i][0] == r - 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r - i;
                                live[i][1] = c;
                            }
                            lives.push_back(live);
                        }
                    }
                }

                // checking South-West
                if (r - 3 >= 0 && c - 3 >= 0) {
                    if (node.currBoard[r - 3][c - 3] == playChar && node.currBoard[r - 1][c - 1] == playChar && node.currBoard[r - 2][c - 2] == playChar)
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or col then no lives which make this one. 
                            if ((l[0][1] - l[1][1]) / (l[0][0] - l[1][0]) != 1)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][0] == r || l[i][1] == c || l[i][0] == r - 1 || l[i][1] == c - 1 ||
                                    l[i][0] == r - 2 || l[i][1] == c - 2 || l[i][0] == r - 3 || l[i][1] == c - 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r - i;
                                live[i][1] = c - i;
                            }
                            lives.push_back(live);
                        }
                    }
                }

                // checking west
                if (c - 3 >= 0) {
                    if (node.currBoard[r][c - 3] == playChar && node.currBoard[r][c - 1] == playChar && node.currBoard[r][c - 1] == playChar)
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or r then no lives which make this one. 
                            if ((l[0][1] - l[1][1]) / (l[0][0] - l[1][0]) != 0 || l[0][0] != r)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][1] == c || l[i][1] == c - 1 || l[i][1] == c - 2 || l[i][1] == c - 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r;
                                live[i][1] = c - i;
                            }
                            lives.push_back(live);
                        }
                    }
                }

                // checking west-north
                if (r + 3 < MAX_ROW && c - 3 >= 0) {
                    if (node.currBoard[r + 3][c - 3] == playChar && node.currBoard[r + 1][c - 1] == playChar && node.currBoard[r + 2][c - 2] == playChar)
                    {
                        bool conflict = false;
                        for (auto& l : lives) {
                            // if its slope does not match or col then no lives which make this one. 
                            if ((l[0][1] - l[1][1]) / (l[0][0] - l[1][0]) != 1)
                                continue;
                            for (uint8_t i = 0; i < 4; i++) {
                                // if point matches, then automatically north r,c is not a live
                                if (l[i][0] == r || l[i][1] == c || l[i][0] == r + 1 || l[i][1] == c - 1 ||
                                    l[i][0] == r + 2 || l[i][1] == c - 2 || l[i][0] == r + 3 || l[i][1] == c - 3) {
                                    conflict = true;
                                    break;
                                }
                            }
                            if (conflict)
                                break;
                        }
                        if (!conflict)
                        {
                            uint8_t live[4][2];
                            for (uint8_t i = 0; i < 4; i++) {
                                live[i][0] = r + i;
                                live[i][1] = c - i;
                            }
                            lives.push_back(live);
                        }

                    }
                }
            }
        }
    }
    return lives.size();
}

// returns the node's score
score NodeScore(const Node& node) {
    return NumLives(node, true);
}

int MAX_DEPTH = 10; Node next_move;
// minimax search algorithm
score Minimax(Node node, int depth, bool isMax, score alpha, score beta) {
    if (depth == MAX_DEPTH || IsLeaf(node))
        return NodeScore(node);

    score bestVal;
    if (isMax) {
        bestVal = MIN_SCORE;
        for (auto& child : PossibleMoves(node, isMax)) {
            score value = Minimax(child, depth + 1, false, alpha, beta);
            bestVal = Max(bestVal, value);
            alpha = Max(alpha, bestVal);
            if (beta <= alpha) {
                for (uint8_t r = 0; r < MAX_ROW; r++) {
                    for (uint8_t c = 0; c < MAX_COL; c++) {
                        next_move.currBoard[r][c] = child.currBoard[r][c];
                    }
                }
                break;
            }
        }
    }
    else {
        bestVal = MAX_SCORE;
        for (auto& child : PossibleMoves(node, isMax)) {
            score value = Minimax(child, depth + 1, true, alpha, beta);
            bestVal = Min(bestVal, value);
            alpha = Min(beta, bestVal);
            if (beta <= alpha) {
                for (uint8_t r = 0; r < MAX_ROW; r++) {
                    for (uint8_t c = 0; c < MAX_COL; c++) {
                        next_move.currBoard[r][c] = child.currBoard[r][c];
                    }
                }
                break;
            }
        }
    }
    return bestVal;
}

// diplays game board
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
            printf(" %c |", node.currBoard[r][c]);
        }
        cout << endl;
    }
    cout << " " << string(2 + 4 * MAX_COL, '-') << endl;
}

void Diff(const Node& OG, const Node& newG, uint8_t* lastPlay) {
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        for (uint8_t c = 0; c < MAX_COL; c++) {
            if (OG.currBoard[r][c] != newG.currBoard[r][c]) {
                lastPlay[0] = r;
                lastPlay[1] = c;
                return;
            }
        }
    }
}

// user input checking
uint8_t LetterToRow(const char& letter) {
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
bool ValidMove(const Node& node, const uint8_t& r, const uint8_t& c) {
    return node.currBoard[r][c] == '\0';
}

int main() {
    // Changing the color of the exe window
    cout << "\tPLEASE HAVE AUDIO ON!!! There is Music Playing in the background. \n\n";
    cout << "\n\nWelcome to Freedom!\n";

    // declare node and print board
    Node node;
    PrintBoard(node);

    bool turn = true;   // true == person, false == computer
    bool over = false;
    while (!over) {
        if (turn) {
            char letter; int col; uint8_t row;
            do {
                printf("Please make a move (RowCol Ex: A3): ");
                cin >> letter >> col;
                col--;
                row = LetterToRow(letter);
                if (row != -1) {
                    if (!ValidMove) {
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
            uint8_t index[2];
            Diff(node, next_move, index);
            node.currBoard[index[0]][index[1]] = next_move.currBoard[index[0]][index[1]];
            node.lastPlay[0] = index[0];
            node.lastPlay[1] = index[1];
        }

        over = IsLeaf(node);

        turn = !turn;
        PrintBoard(node);
    }
    return 0;
}