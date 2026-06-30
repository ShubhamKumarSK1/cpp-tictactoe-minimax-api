#include "Player.h"
#include <iostream>
#include <limits>
#include <algorithm>

// --- Base Player ---
Player::Player(std::string n, char m) : name(n), marker(m) {}
Player::~Player() {}
char Player::getMarker() const { return marker; }
std::string Player::getName() const { return name; }

// --- Human Player ---
HumanPlayer::HumanPlayer(std::string n, char m) : Player(n, m) {}

int HumanPlayer::getMove(const Board& board) {
    int move;
    while (true) {
        std::cout << name << " (" << marker << "), enter your move (1-9): ";
        std::cout.flush();
        std::cin >> move;
        
        if (std::cin.good() && board.isValidMove(move - 1)) {
            return move - 1;
        }
        
        std::cout << "Invalid move! Spot taken or out of bounds. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// --- AI Player (Minimax) ---
AIPlayer::AIPlayer(std::string n, char m, char oppM) : Player(n, m), opponentMarker(oppM) {}

int AIPlayer::minimax(Board& board, int depth, bool isMaximizing) {
    if (board.checkWin(this->marker)) return 10 - depth;
    if (board.checkWin(opponentMarker)) return depth - 10;
    if (board.checkDraw()) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 9; ++i) {
            if (board.isValidMove(i)) {
                board.makeMove(i, this->marker);
                int score = minimax(board, depth + 1, false);
                board.undoMove(i, i + 1);
                bestScore = std::max(score, bestScore);
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 9; ++i) {
            if (board.isValidMove(i)) {
                board.makeMove(i, opponentMarker);
                int score = minimax(board, depth + 1, true);
                board.undoMove(i, i + 1);
                bestScore = std::min(score, bestScore);
            }
        }
        return bestScore;
    }
}

int AIPlayer::getMove(const Board& board) {
    std::cout << name << " is thinking...\n";
    Board tempBoard = board; 
    int bestMove = -1;
    int bestScore = -1000;

    for (int i = 0; i < 9; ++i) {
        if (tempBoard.isValidMove(i)) {
            tempBoard.makeMove(i, this->marker);
            int score = minimax(tempBoard, 0, false);
            tempBoard.undoMove(i, i + 1);

            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}