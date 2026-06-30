#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.h"

// Base Abstract Class
class Player {
protected:
    char marker;
    std::string name;

public:
    Player(std::string n, char m);
    virtual ~Player();

    char getMarker() const;
    std::string getName() const;
    virtual int getMove(const Board& board) = 0; // Pure Virtual
};

// Human Subclass
class HumanPlayer : public Player {
public:
    HumanPlayer(std::string n, char m);
    int getMove(const Board& board) override;
};

// Minimax AI Subclass
class AIPlayer : public Player {
private:
    char opponentMarker;
    int minimax(Board& board, int depth, bool isMaximizing);

public:
    AIPlayer(std::string n, char m, char oppM);
    int getMove(const Board& board) override;
};

#endif