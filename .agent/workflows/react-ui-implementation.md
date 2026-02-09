---
description: Implementing React UI for Tic-Tac-Toe
---

# Implementing a React UI for Your Tic-Tac-Toe Game

This guide walks you through adding a visual React-based user interface to your C++ tic-tac-toe game.

## Architecture Overview

You have two main options:

### **Option 1: Pure React Implementation (Recommended for Beginners)**

- Reimplement the game logic in JavaScript/React
- Completely browser-based, no C++ backend needed
- Easier to deploy and share
- **Best for learning React and web development**

### **Option 2: React Frontend + C++ Backend**

- Keep C++ game logic as a REST API server
- React frontend communicates via HTTP requests
- More complex but demonstrates full-stack architecture
- Requires additional libraries (e.g., cpp-httplib, Crow, or Pistache)

---

## Option 1: Pure React Implementation (Recommended)

### Step 1: Set Up React Project

```bash
# Navigate to your project directory
cd /Users/ericpham/Documents/GitHub/ticTacToeBeginner

# Create a new React app in a subdirectory
npx create-react-app tic-tac-toe-ui

# Navigate into the React app
cd tic-tac-toe-ui
```

### Step 2: Create the Game Component

Create `src/TicTacToe.jsx`:

```jsx
import React, { useState } from "react";
import "./TicTacToe.css";

function TicTacToe() {
  const [board, setBoard] = useState(Array(9).fill(null));
  const [currentPlayer, setCurrentPlayer] = useState("X");
  const [winner, setWinner] = useState(null);
  const [gameOver, setGameOver] = useState(false);

  const checkWinner = (squares) => {
    const lines = [
      [0, 1, 2],
      [3, 4, 5],
      [6, 7, 8], // rows
      [0, 3, 6],
      [1, 4, 7],
      [2, 5, 8], // columns
      [0, 4, 8],
      [2, 4, 6], // diagonals
    ];

    for (let line of lines) {
      const [a, b, c] = line;
      if (
        squares[a] &&
        squares[a] === squares[b] &&
        squares[a] === squares[c]
      ) {
        return squares[a];
      }
    }
    return null;
  };

  const handleClick = (index) => {
    if (board[index] || winner || gameOver) return;

    const newBoard = [...board];
    newBoard[index] = currentPlayer;
    setBoard(newBoard);

    const gameWinner = checkWinner(newBoard);
    if (gameWinner) {
      setWinner(gameWinner);
      setGameOver(true);
    } else if (newBoard.every((cell) => cell !== null)) {
      setGameOver(true);
    } else {
      setCurrentPlayer(currentPlayer === "X" ? "O" : "X");
    }
  };

  const resetGame = () => {
    setBoard(Array(9).fill(null));
    setCurrentPlayer("X");
    setWinner(null);
    setGameOver(false);
  };

  return (
    <div className="game-container">
      <h1>Tic-Tac-Toe</h1>

      <div className="status">
        {winner ?
          <p className="winner-text">Player {winner} wins! 🎉</p>
        : gameOver ?
          <p className="draw-text">It's a draw! 🤝</p>
        : <p>Player {currentPlayer}'s turn</p>}
      </div>

      <div className="board">
        {board.map((cell, index) => (
          <button
            key={index}
            className={`cell ${cell ? "filled" : ""}`}
            onClick={() => handleClick(index)}
            disabled={!!cell || gameOver}
          >
            {cell}
          </button>
        ))}
      </div>

      <button className="reset-button" onClick={resetGame}>
        New Game
      </button>
    </div>
  );
}

export default TicTacToe;
```

### Step 3: Create Styling

Create `src/TicTacToe.css`:

```css
.game-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  min-height: 100vh;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  font-family:
    "Inter",
    -apple-system,
    BlinkMacSystemFont,
    "Segoe UI",
    sans-serif;
  color: white;
}

h1 {
  font-size: 3rem;
  margin-bottom: 1rem;
  text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);
}

.status {
  margin-bottom: 2rem;
  font-size: 1.5rem;
  font-weight: 600;
}

.winner-text {
  color: #ffd700;
  animation: pulse 1s ease-in-out infinite;
}

.draw-text {
  color: #90caf9;
}

@keyframes pulse {
  0%,
  100% {
    transform: scale(1);
  }
  50% {
    transform: scale(1.05);
  }
}

.board {
  display: grid;
  grid-template-columns: repeat(3, 120px);
  grid-template-rows: repeat(3, 120px);
  gap: 10px;
  margin-bottom: 2rem;
  padding: 20px;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 20px;
  backdrop-filter: blur(10px);
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
}

.cell {
  background: rgba(255, 255, 255, 0.9);
  border: none;
  border-radius: 15px;
  font-size: 3rem;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.3s ease;
  color: #667eea;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.cell:hover:not(:disabled) {
  background: white;
  transform: translateY(-5px);
  box-shadow: 0 8px 12px rgba(0, 0, 0, 0.2);
}

.cell:active:not(:disabled) {
  transform: translateY(-2px);
}

.cell:disabled {
  cursor: not-allowed;
  opacity: 0.9;
}

.cell.filled {
  animation: fillCell 0.3s ease;
}

@keyframes fillCell {
  0% {
    transform: scale(0);
  }
  50% {
    transform: scale(1.2);
  }
  100% {
    transform: scale(1);
  }
}

.reset-button {
  padding: 15px 40px;
  font-size: 1.2rem;
  font-weight: 600;
  background: white;
  color: #667eea;
  border: none;
  border-radius: 50px;
  cursor: pointer;
  transition: all 0.3s ease;
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
}

.reset-button:hover {
  transform: translateY(-3px);
  box-shadow: 0 6px 20px rgba(0, 0, 0, 0.3);
  background: #f0f0f0;
}

.reset-button:active {
  transform: translateY(-1px);
}
```

### Step 4: Update App.js

Replace `src/App.js`:

```jsx
import React from "react";
import TicTacToe from "./TicTacToe";
import "./App.css";

function App() {
  return (
    <div className="App">
      <TicTacToe />
    </div>
  );
}

export default App;
```

### Step 5: Update App.css

Replace `src/App.css`:

```css
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  margin: 0;
  font-family:
    "Inter",
    -apple-system,
    BlinkMacSystemFont,
    "Segoe UI",
    "Roboto",
    sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
}

.App {
  min-height: 100vh;
}
```

### Step 6: Run the React App

```bash
# Make sure you're in the tic-tac-toe-ui directory
npm start
```

Your React app will open at `http://localhost:3000`!

---

## Option 2: React Frontend + C++ Backend

### Overview

This approach requires:

1. Converting your C++ game into a REST API server
2. Creating a React frontend that makes HTTP requests
3. Handling CORS and state management

### Step 1: Install C++ HTTP Library

You'll need to add a C++ HTTP server library. **Crow** is a good choice:

```bash
# Install Crow (header-only library)
cd /Users/ericpham/Documents/GitHub/ticTacToeBeginner
git clone https://github.com/CrowCpp/Crow.git
```

### Step 2: Modify CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(TicTacToeServer)

set(CMAKE_CXX_STANDARD 17)

# Include Crow
include_directories(${CMAKE_SOURCE_DIR}/Crow/include)

# Find Boost (required by Crow)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(server server.cpp)
```

### Step 3: Create C++ REST API Server

Create `server.cpp`:

```cpp
#include "crow_all.h"
#include <string>
#include <vector>
#include <array>

class TicTacToeGame {
private:
    std::array<char, 9> board;
    char currentPlayer;

public:
    TicTacToeGame() : currentPlayer('X') {
        board.fill(' ');
    }

    bool makeMove(int position) {
        if (position < 0 || position > 8 || board[position] != ' ') {
            return false;
        }
        board[position] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        return true;
    }

    char checkWinner() {
        // Check rows, columns, diagonals
        int lines[8][3] = {
            {0,1,2}, {3,4,5}, {6,7,8}, // rows
            {0,3,6}, {1,4,7}, {2,5,8}, // columns
            {0,4,8}, {2,4,6}           // diagonals
        };

        for (auto& line : lines) {
            if (board[line[0]] != ' ' &&
                board[line[0]] == board[line[1]] &&
                board[line[1]] == board[line[2]]) {
                return board[line[0]];
            }
        }
        return ' ';
    }

    std::string getBoardState() {
        return std::string(board.begin(), board.end());
    }

    void reset() {
        board.fill(' ');
        currentPlayer = 'X';
    }
};

int main() {
    crow::SimpleApp app;
    TicTacToeGame game;

    // Enable CORS
    CROW_ROUTE(app, "/api/move").methods("POST"_method)
    ([&game](const crow::request& req) {
        auto body = crow::json::load(req.body);
        int position = body["position"].i();

        if (game.makeMove(position)) {
            crow::json::wvalue response;
            response["success"] = true;
            response["board"] = game.getBoardState();
            response["winner"] = std::string(1, game.checkWinner());
            return crow::response(200, response);
        }
        return crow::response(400, "Invalid move");
    });

    CROW_ROUTE(app, "/api/reset").methods("POST"_method)
    ([&game]() {
        game.reset();
        return crow::response(200, "Game reset");
    });

    app.port(8080).multithreaded().run();
}
```

### Step 4: Create React Frontend with API Integration

Modify `src/TicTacToe.jsx` to use fetch:

```jsx
const handleClick = async (index) => {
  if (board[index] || winner || gameOver) return;

  try {
    const response = await fetch("http://localhost:8080/api/move", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ position: index }),
    });

    const data = await response.json();
    if (data.success) {
      setBoard(data.board.split(""));
      if (data.winner !== " ") {
        setWinner(data.winner);
        setGameOver(true);
      }
    }
  } catch (error) {
    console.error("Error making move:", error);
  }
};
```

---

## Recommendation

**Start with Option 1** (Pure React) because:

- ✅ Faster to implement
- ✅ No additional C++ dependencies
- ✅ Easier to deploy and share
- ✅ Better for learning React
- ✅ No CORS or networking complexity

You can always add a C++ backend later if you want to learn full-stack development!

## Next Steps

1. Choose your approach (Option 1 recommended)
2. Follow the steps above
3. Customize the styling to your preference
4. Add features like score tracking, player names, or AI opponent
