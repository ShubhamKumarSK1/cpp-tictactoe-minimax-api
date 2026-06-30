#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "Board.h"
#include "Player.h"

#pragma comment(lib, "ws2_32.lib")

const int PORT = 8080;

// Simple helper to construct a manual JSON string response
std::string buildResponseJSON(const Board& board, const std::string& status, const std::string& winner) {
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"status\": \"" << status << "\",\n";
    ss << "  \"winner\": \"" << winner << "\",\n";
    ss << "  \"board\": [";
    
    const auto& grid = board.getGrid();
    for (size_t i = 0; i < grid.size(); ++i) {
        ss << "\"" << grid[i] << "\"";
        if (i < grid.size() - 1) ss << ", ";
    }
    ss << "]\n";
    ss << "}\n";
    return ss.str();
}

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed.\n";
        return 1;
    }

    // 2. Create the Listening Socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    // 3. Bind and Listen
    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Tic-Tac-Toe Minimax TCP Engine Server running on port " << PORT << "...\n";

    // 4. Server Loop: Accept inbound requests
    while (true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) continue;

        char recvBuffer[1024] = {0};
        int bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
        
        if (bytesReceived > 0) {
            std::string incomingData(recvBuffer);
            
            // Reconstruct the board from any text input containing board configuration
            // Look for characters 'X', 'O', or digits '1'-'9'
            Board currentBoard;
            int cellIdx = 0;
            
            // Parse through string to pick out the 9 board slots
            for (char ch : incomingData) {
                if (cellIdx < 9 && (ch == 'X' || ch == 'O' || (ch >= '1' && ch <= '9'))) {
                    currentBoard.makeMove(cellIdx, ch);
                    cellIdx++;
                }
            }

            AIPlayer ai("Minimax AI", 'O', 'X'); 
            std::string gameStatus = "ongoing";
            std::string winner = "none";

            // Process AI calculation steps
            if (currentBoard.checkWin('X')) {
                gameStatus = "game_over";
                winner = "X";
            } else if (currentBoard.checkDraw()) {
                gameStatus = "draw";
            } else {
                int bestMove = ai.getMove(currentBoard);
                if (bestMove != -1) {
                    currentBoard.makeMove(bestMove, 'O');
                    if (currentBoard.checkWin('O')) {
                        gameStatus = "game_over";
                        winner = "O";
                    } else if (currentBoard.checkDraw()) {
                        gameStatus = "draw";
                    }
                }
            }

            // Generate clean response string and return it
            std::string outJson = buildResponseJSON(currentBoard, gameStatus, winner);
            send(clientSocket, outJson.c_str(), static_cast<int>(outJson.length()), 0);
        }

        closesocket(clientSocket);
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}