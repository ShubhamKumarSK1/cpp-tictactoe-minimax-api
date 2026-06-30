#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:
    std::vector<char> grid;

public:
    Board();
    void display() const;
    bool isValidMove(int cell) const;
    void makeMove(int cell, char marker);
    void undoMove(int cell, int originalVal);
    bool checkWin(char player) const;
    bool checkDraw() const;
    const std::vector<char>& getGrid() const;
};

#endif