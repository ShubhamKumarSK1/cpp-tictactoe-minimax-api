#include <iostream>
#include "Player.h"
#include "Game.h"

int main() {
    std::cout << "Welcome to Multi-File OOP Tic-Tac-Toe!\n";

    HumanPlayer human("Human Player", 'X');
    AIPlayer ai("Minimax AI", 'O', 'X');

    Game tictactoe(&human, &ai);
    tictactoe.play();

    return 0;
}