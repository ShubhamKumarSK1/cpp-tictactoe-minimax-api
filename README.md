# C++ Minimax Tic-Tac-Toe AI with Python GUI

A decoupled, high-performance client-server Tic-Tac-Toe game. The core decision-making brain uses an unbeatable **Minimax Search Algorithm** written in **C++**, which runs as a background TCP socket server. The presentation layer is a desktop application built in **Python (Tkinter)** that communicates over a local network pipeline (`localhost:8080`).

---

## 🏗️ Architecture Design

- **Backend (C++):** Manages game logic, board state representation, validation, and recursive minimax tree-search operations. It compiles as an independent native binary.
- **Network Pipeline (TCP Sockets):** Custom, lightweight network protocol using Windows Native Sockets (`<winsock2.h>`). Decouples logic from UI, passing raw text strings and returning structured JSON payloads.
- **Frontend (Python):** Event-driven desktop UI (`tkinter`) that tracks human mouse interactions, packages layouts, handles network socket streaming, and renders server updates.

---

## 📁 File Structure

```text
├── Board.h          # Board class interface
├── Board.cpp        # Board grid configuration, validation, & win-checks
├── Player.h         # Abstract Player class, Human and AI definitions
├── Player.cpp       # Pure virtual implementation & Minimax search tree
├── api_main.cpp     # Native Windows Socket (Winsock) API server listener
├── gui.py           # Python Tkinter Desktop Interface
└── .gitignore       # Excludes binaries from repository tracking
# cpp-tictactoe-minimax-api
