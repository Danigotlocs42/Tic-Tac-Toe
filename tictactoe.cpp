#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const char HUMAN = 'X';
const char AI = 'O';
const char EMPTY = ' ';

// Function prototypes
void printBoard(const vector<vector<char>> &board);
bool isMovesLeft(const vector<vector<char>> &board);
int evaluate(const vector<vector<char>> &board);
int minimax(vector<vector<char>> &board, int depth, bool isMax, int alpha, int beta, int maxDepth);
pair<int, int> findBestMove(vector<vector<char>> &board, int maxDepth);
void playGame(int difficulty);

int main() {
    while (true) {
        cout << "Welcome to Tic Tac Toe!" << endl;
        cout << "1. Play Game" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int difficulty;
            cout << "Choose difficulty level (1: Easy, 2: Medium, 3: Hard): ";
            cin >> difficulty;
            if (difficulty >= 1 && difficulty <= 3) {
                playGame(difficulty);
            } else {
                cout << "Invalid difficulty level. Returning to menu." << endl;
            }
        } else if (choice == 2) {
            cout << "Exiting the game. Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}

// Function to print the board
void printBoard(const vector<vector<char>> &board) {
    cout << endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << " " << board[i][j] << " ";
            if (j < 2) cout << "|"; // Add a separator for columns
        }
        cout << endl;
        if (i < 2) cout << "---|---|---" << endl; // Add a separator for rows
    }
    cout << endl;
}


// Check if there are moves left
bool isMovesLeft(const vector<vector<char>> &board) {
    for (const auto &row : board) {
        for (char cell : row) {
            if (cell == EMPTY) return true;
        }
    }
    return false;
}

// Evaluate the board
int evaluate(const vector<vector<char>> &board) {
    // Check rows and columns
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == AI) return 10;
            if (board[i][0] == HUMAN) return -10;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == AI) return 10;
            if (board[0][i] == HUMAN) return -10;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == AI) return 10;
        if (board[0][0] == HUMAN) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == AI) return 10;
        if (board[0][2] == HUMAN) return -10;
    }

    return 0;
}

// Minimax algorithm with alpha-beta pruning
int minimax(vector<vector<char>> &board, int depth, bool isMax, int alpha, int beta, int maxDepth) {
    int score = evaluate(board);

    // Base cases
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (!isMovesLeft(board) || depth == maxDepth) return 0;

    if (isMax) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = AI;
                    best = max(best, minimax(board, depth + 1, false, alpha, beta, maxDepth));
                    board[i][j] = EMPTY;
                    alpha = max(alpha, best);
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = HUMAN;
                    best = min(best, minimax(board, depth + 1, true, alpha, beta, maxDepth));
                    board[i][j] = EMPTY;
                    beta = min(beta, best);
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    }
}

// Find the best move for the AI
pair<int, int> findBestMove(vector<vector<char>> &board, int maxDepth) {
    int bestVal = numeric_limits<int>::min();
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == EMPTY) {
                board[i][j] = AI;
                int moveVal = minimax(board, 0, false, numeric_limits<int>::min(), numeric_limits<int>::max(), maxDepth);
                board[i][j] = EMPTY;
                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

// Play the game
void playGame(int difficulty) {
    vector<vector<char>> board(3, vector<char>(3, EMPTY));
    int maxDepth = difficulty * 2; // Adjust depth based on difficulty
    int turn = 0; // 0: HUMAN, 1: AI
    string input;

    cout << "You are 'X', and the AI is 'O'." << endl;
    cout << "Rows: (0, 1, 2) from left to right." << endl;
    cout << "Columns (0, 1, 2) from top to bottom." << endl;
    cout << "To make a move, type the row and column numbers separated by a space." << endl;
    cout << "For example, typing '1 1' marks the middle cell." << endl;

    while (true) {
        // Print the board without coordinates
        cout << "\nCurrent Board:\n";
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                cout << " " << board[i][j] << " ";
                if (j < 2) cout << "|"; // Column separator
            }
            cout << endl;
            if (i < 2) cout << "---|---|---" << endl; // Row separator
        }

        if (evaluate(board) == 10) {
            cout << "AI wins!" << endl;
            break;
        } else if (evaluate(board) == -10) {
            cout << "You win!" << endl;
            break;
        } else if (!isMovesLeft(board)) {
            cout << "It's a tie!" << endl;
            break;
        }

        if (turn == 0) {
            cout << "Enter your move (row and column, or 'q' to quit): ";
            cin >> input;

            // Check for quit option
            if (input == "q" || input == "Q") {
                cout << "You chose to quit. Returning to the main menu..." << endl;
                return; // Return to main menu
            }

            // Parse row and column from input
            int row, col;
            try {
                row = stoi(input);
                cin >> col;
            } catch (...) {
                cout << "Invalid input. Try again." << endl;
                continue;
            }

            if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == EMPTY) {
                board[row][col] = HUMAN;
                turn = 1; // Switch turn to AI
            } else {
                cout << "Invalid move. Try again." << endl;
            }
        } else {
            pair<int, int> bestMove = findBestMove(board, maxDepth);
            board[bestMove.first][bestMove.second] = AI;
            turn = 0; // Switch turn to HUMAN
        }
    }
}
