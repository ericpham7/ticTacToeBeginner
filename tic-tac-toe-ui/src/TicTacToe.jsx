import React, { useState, useEffect } from "react";
import "./TicTacToe.css";

const API_URL = "http://localhost:8080/api";

function TicTacToe() {
  const [board, setBoard] = useState(Array(9).fill(" "));
  const [currentPlayer, setCurrentPlayer] = useState("X");
  const [winner, setWinner] = useState(" ");
  const [gameOver, setGameOver] = useState(false);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  // Fetch initial game state when component mounts
  useEffect(() => {
    fetchGameState();
  }, []);

  // Fetch game state from C++ backend
  const fetchGameState = async () => {
    try {
      const response = await fetch(`${API_URL}/state`);
      const data = await response.json();
      updateGameState(data);
    } catch (err) {
      setError(
        "Failed to connect to server. Make sure the C++ server is running on port 8080.",
      );
      console.error("Error fetching game state:", err);
    }
  };

  // Update local state from server response
  const updateGameState = (data) => {
    setBoard(data.board);
    setCurrentPlayer(data.currentPlayer);
    setWinner(data.winner);
    setGameOver(data.gameOver);
    setError(null);
  };

  // Handle cell click - send move to backend
  const handleClick = async (index) => {
    if (board[index] !== " " || gameOver || loading) return;

    setLoading(true);
    try {
      const response = await fetch(`${API_URL}/move`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ position: index }),
      });

      if (response.ok) {
        const data = await response.json();
        updateGameState(data);
      } else {
        const errorData = await response.json();
        setError(errorData.error || "Invalid move");
      }
    } catch (err) {
      setError("Failed to make move. Check if server is running.");
      console.error("Error making move:", err);
    } finally {
      setLoading(false);
    }
  };

  // Reset game - send reset request to backend
  const resetGame = async () => {
    setLoading(true);
    try {
      const response = await fetch(`${API_URL}/reset`, {
        method: "POST",
      });

      if (response.ok) {
        const data = await response.json();
        updateGameState(data);
      }
    } catch (err) {
      setError("Failed to reset game. Check if server is running.");
      console.error("Error resetting game:", err);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="game-container">
      <div className="game-header">
        <h1>🎮 Tic-Tac-Toe</h1>
        <p className="subtitle">Powered by C++ Backend</p>
      </div>

      {error && <div className="error-message">⚠️ {error}</div>}

      <div className="status">
        {winner !== " " ?
          <p className="winner-text">Player {winner} wins! 🎉</p>
        : gameOver ?
          <p className="draw-text">It's a draw! 🤝</p>
        : <p>
            Player <span className="current-player">{currentPlayer}</span>'s
            turn
          </p>
        }
      </div>

      <div className="board">
        {board.map((cell, index) => (
          <button
            key={index}
            className={`cell ${cell !== " " ? "filled" : ""} ${
              cell === "X" ? "player-x"
              : cell === "O" ? "player-o"
              : ""
            }`}
            onClick={() => handleClick(index)}
            disabled={cell !== " " || gameOver || loading}
          >
            {cell !== " " ? cell : ""}
          </button>
        ))}
      </div>

      <button className="reset-button" onClick={resetGame} disabled={loading}>
        {loading ? "⏳ Loading..." : "🔄 New Game"}
      </button>

      <div className="info-panel">
        <p>
          <strong>Backend Status:</strong>{" "}
          <span className={error ? "status-offline" : "status-online"}>
            {error ? "🔴 Offline" : "🟢 Online"}
          </span>
        </p>
        <p className="tech-stack">React ⚛️ + C++ 🚀</p>
      </div>
    </div>
  );
}

export default TicTacToe;
