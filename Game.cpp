#include "Game.h"
#include <iostream>

Game::Game(Player* p1, Player* p2) : player1(p1), player2(p2) {
    currentTurn = player1;
}

void Game::play() {
    std::cout << "\n=== MATCH START ===\n";
    bool gameRunning = true;

    while (gameRunning) {
        board.display();
        
        int move = currentTurn->getMove(board);
        board.makeMove(move, currentTurn->getMarker());

        if (board.checkWin(currentTurn->getMarker())) {
            board.display();
            std::cout << "Game Over! " << currentTurn->getName() << " wins! 🏆\n";
            gameRunning = false;
        } else if (board.checkDraw()) {
            board.display();
            std::cout << "Game Over! It's an unbreakable draw! 🤝\n";
            gameRunning = false;
        } else {
            currentTurn = (currentTurn == player1) ? player2 : player1;
        }
    }
}