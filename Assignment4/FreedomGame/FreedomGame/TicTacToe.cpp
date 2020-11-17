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

// returns the node's score
score NodeScore(const Node& node) {
    return 1;
}

int MAX_DEPTH = 5; Node* next_move = nullptr;
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
                next_move = &child;
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
                next_move = &child;
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
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        printf("%i |", r);
    }
    cout << endl;
    for (uint8_t r = 0; r < MAX_ROW; r++) {
        cout << " " << string(14, '-') << endl;
        printf("%c| ", letter++);
        for (uint8_t c = 0; c < MAX_COL; c++) {
            printf("%c |", node.currBoard[r][c]);
        }
        cout << endl;
    }
    cout << " " << string(14, '-') << endl;
}

int main() {
    // Changing the color of the exe window
    system("color f0");
    cout << "\tPLEASE HAVE AUDIO ON!!! There is Music Playing in the background. \n\n";
    cout << "\n\nWelcome to Freedom!\n";

    // declare node and print board
    Node node;
    PrintBoard(node);

    //bool turn = true;   // true == person, false == computer
    //bool over = false;
    //while (!over) {
    //    if (turn) {
    //        char letter; int move;
    //        printf("Please make a move (RowCol Ex: A3): ");
    //        cin >> letter >> move;
    //    }
    //    else {
    //        Minimax(node, 0, true, MIN_SCORE, MAX_SCORE);

    //    }
    //}
    return 0;
}