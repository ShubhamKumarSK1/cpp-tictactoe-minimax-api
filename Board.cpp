#include "Board.h"
#include <iostream>

Board::Board() {
    grid = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
}

void Board::display() const {
    std::cout << "\n";
    std::cout << " " << grid[0] << " | " << grid[1] << " | " << grid[2] << " \n";
    std::cout << "---|---|---\n";
    std::cout << " " << grid[3] << " | " << grid[4] << " | " << grid[5] << " \n";
    std::cout << "---|---|---\n";
    std::cout << " " << grid[6] << " | " << grid[7] << " | " << grid[8] << " \n";
    std::cout << "\n";
}

bool Board::isValidMove(int cell) const {
    return (cell >= 0 && cell < 9 && grid[cell] != 'X' && grid[cell] != 'O');
}

void Board::makeMove(int cell, char marker) {
    grid[cell] = marker;
}

void Board::undoMove(int cell, int originalVal) {
    grid[cell] = '0' + originalVal;
}

bool Board::checkWin(char player) const {
    const int winConditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; ++i) {
        if (grid[winConditions[i][0]] == player &&
            grid[winConditions[i][1]] == player &&
            grid[winConditions[i][2]] == player) {
            return true;
        }
    }
    return false;
}

bool Board::checkDraw() const {
    for (char cell : grid) {
        if (cell != 'X' && cell != 'O') return false;
    }
    return true;
}

const std::vector<char>& Board::getGrid() const {
    return grid;
}