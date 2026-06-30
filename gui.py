import socket
import tkinter as tk
from tkinter import messagebox

# The board state expected by your C++ engine
board_state = ["1", "2", "3", "4", "5", "6", "7", "8", "9"]
buttons = []

def send_to_cpp(board_string):
    """Opens a pipeline to your C++ server and returns the raw JSON string"""
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('127.0.0.1', 8080))
        s.sendall(board_string.encode('ascii'))
        response = s.recv(1024).decode('ascii')
        s.close()
        return response
    except ConnectionRefusedError:
        messagebox.showerror("Error", "Could not connect to C++ Server! Is tictactoe_api.exe running?")
        return None

def parse_cpp_response(response_raw):
    """Extracts status, winner, and board grid from the manual JSON response"""
    status, winner, board = "ongoing", "none", []
    
    for line in response_raw.splitlines():
        if '"status"' in line:
            status = line.split('"')[3]
        elif '"winner"' in line:
            winner = line.split('"')[3]
        elif '"board"' in line:
            extracted = line.split("[")[1].split("]")[0]
            board = [x.strip().replace('', '').replace('"', '') for x in extracted.split(",")]
            
    return status, winner, board

def update_ui_grid():
    """Syncs the visual buttons with the current background array"""
    for i in range(9):
        val = board_state[i]
        if val in ['X', 'O']:
            buttons[i].config(text=val, state="disabled", disabledforeground="#fff")
            if val == 'X':
                buttons[i].config(bg="#3498db") # Blue for User
            else:
                buttons[i].config(bg="#e74c3c") # Red for AI
        else:
            buttons[i].config(text="", bg="#34495e", state="normal")

def handle_click(index):
    global board_state
    
    # 1. Human registers move ('X')
    board_state[index] = "X"
    update_ui_grid()
    
    # 2. Package board string and ship it off to C++ over localhost pipeline
    payload = "".join(board_state)
    response = send_to_cpp(payload)
    if not response: return

    # 3. Decode what the C++ engine calculated
    status, winner, new_board = parse_cpp_response(response)
    board_state = new_board
    update_ui_grid()
    
    # 4. Handle End Game triggers
    if status == "game_over":
        messagebox.showinfo("Match Result", f"Game Over! Winner: {winner} 🎉" if winner == "X" else "Game Over! The Minimax AI wins! 🤖")
        reset_game()
    elif status == "draw":
        messagebox.showinfo("Match Result", "It's an unbreakable Draw! 🤝")
        reset_game()

def reset_game():
    global board_state
    board_state = ["1", "2", "3", "4", "5", "6", "7", "8", "9"]
    update_ui_grid()

# --- Build the Window Layout ---
root = tk.Tk()
root.title("C++ Minimax GUI Framework")
root.configure(bg="#2c3e50")
root.resizable(False, False)

# Main container grid
grid_frame = tk.Frame(root, bg="#2c3e50", padx=10, pady=10)
grid_frame.pack()

# Generate 9 distinct layout boxes dynamically
for i in range(9):
    btn = tk.Button(
        grid_frame, text="", font=("Arial", 24, "bold"), 
        width=5, height=2, bg="#34495e", fg="white",
        activebackground="#2c3e50",
        command=lambda idx=i: handle_click(idx)
    )
    row = i // 3
    col = i % 3
    btn.grid(row=row, column=col, padx=5, pady=5)
    buttons.append(btn)

update_ui_grid()
root.mainloop()