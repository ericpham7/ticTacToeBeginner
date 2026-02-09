#include "httplib.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>

// Simple JSON builder (since we're avoiding external dependencies)
class SimpleJSON {
public:
  static std::string arrayToJSON(const std::array<char, 9> &board) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < board.size(); i++) {
      ss << "\"" << board[i] << "\"";
      if (i < board.size() - 1)
        ss << ",";
    }
    ss << "]";
    return ss.str();
  }

  static std::string gameStateToJSON(const std::array<char, 9> &board,
                                     char currentPlayer, char winner,
                                     bool gameOver) {
    std::stringstream ss;
    ss << "{";
    ss << "\"board\":" << arrayToJSON(board) << ",";
    ss << "\"currentPlayer\":\"" << currentPlayer << "\",";
    ss << "\"winner\":\"" << winner << "\",";
    ss << "\"gameOver\":" << (gameOver ? "true" : "false");
    ss << "}";
    return ss.str();
  }

  static int extractPosition(const std::string &json) {
    // Simple JSON parser for {"position": N}
    size_t pos = json.find("\"position\"");
    if (pos == std::string::npos)
      return -1;

    size_t colonPos = json.find(":", pos);
    if (colonPos == std::string::npos)
      return -1;

    size_t numStart = json.find_first_of("0123456789", colonPos);
    if (numStart == std::string::npos)
      return -1;

    return std::stoi(json.substr(numStart));
  }
};

// Game class to manage tic-tac-toe logic
class TicTacToeGame {
private:
  std::array<char, 9> board;
  char currentPlayer;
  char winner;
  bool gameOver;

  // Check if there's a winner
  char checkWinner() {
    // Winning combinations: rows, columns, diagonals
    const int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // columns
        {0, 4, 8}, {2, 4, 6}             // diagonals
    };

    for (const auto &pattern : winPatterns) {
      if (board[pattern[0]] != ' ' && board[pattern[0]] == board[pattern[1]] &&
          board[pattern[1]] == board[pattern[2]]) {
        return board[pattern[0]];
      }
    }
    return ' ';
  }

  // Check if board is full (draw)
  bool isBoardFull() {
    return std::none_of(board.begin(), board.end(),
                        [](char c) { return c == ' '; });
  }

public:
  TicTacToeGame() { reset(); }

  // Make a move at the specified position
  bool makeMove(int position) {
    if (position < 0 || position > 8 || board[position] != ' ' || gameOver) {
      return false;
    }

    board[position] = currentPlayer;

    // Check for winner
    winner = checkWinner();
    if (winner != ' ') {
      gameOver = true;
    } else if (isBoardFull()) {
      gameOver = true;
    } else {
      // Switch player
      currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return true;
  }

  // Get current game state as JSON string
  std::string getGameState() {
    return SimpleJSON::gameStateToJSON(board, currentPlayer, winner, gameOver);
  }

  // Reset the game
  void reset() {
    board.fill(' ');
    currentPlayer = 'X';
    winner = ' ';
    gameOver = false;
  }
};

int main() {
  using namespace httplib;

  Server svr;
  TicTacToeGame game;

  // Enable CORS for all routes
  svr.set_default_headers(
      {{"Access-Control-Allow-Origin", "http://localhost:3000"},
       {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
       {"Access-Control-Allow-Headers", "Content-Type"}});

  // Handle OPTIONS requests (CORS preflight)
  svr.Options("/api/.*", [](const Request &req, Response &res) {
    res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.status = 200;
  });

  // Route: GET /api/state - Get current game state
  svr.Get("/api/state", [&game](const Request &req, Response &res) {
    res.set_content(game.getGameState(), "application/json");
  });

  // Route: POST /api/move - Make a move
  svr.Post("/api/move", [&game](const Request &req, Response &res) {
    int position = SimpleJSON::extractPosition(req.body);

    if (position == -1) {
      res.status = 400;
      res.set_content("{\"error\":\"Invalid JSON\"}", "application/json");
      return;
    }

    if (game.makeMove(position)) {
      res.set_content(game.getGameState(), "application/json");
    } else {
      res.status = 400;
      res.set_content("{\"error\":\"Invalid move\"}", "application/json");
    }
  });

  // Route: POST /api/reset - Reset the game
  svr.Post("/api/reset", [&game](const Request &req, Response &res) {
    game.reset();
    res.set_content(game.getGameState(), "application/json");
  });

  // Start server
  std::cout << "🎮 Tic-Tac-Toe Server starting on http://localhost:8080"
            << std::endl;
  std::cout << "📡 API Endpoints:" << std::endl;
  std::cout << "   GET  /api/state  - Get game state" << std::endl;
  std::cout << "   POST /api/move   - Make a move" << std::endl;
  std::cout << "   POST /api/reset  - Reset game" << std::endl;
  std::cout << "\n✨ Server is ready! Start your React frontend on port 3000\n"
            << std::endl;

  svr.listen("0.0.0.0", 8080);

  return 0;
}
