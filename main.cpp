#include <iostream>

using namespace std;

void ticTacToe();

int main() {

    ticTacToe();

    return 0;
}
void ticTacToe(){

    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };


    char playerX = 'X';
    char playerO = 'O';
    char currentPlayer = playerX;
    char winner = ' ';

    int r;
    int c;


    for(int i = 0; i < 9; i++) {
        cout << "   |   |   " << endl;
        cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << " " << endl;
        cout << "___|___|___" << endl;
        cout << "   |   |   " << endl;
        cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << " " << endl;
        cout << "___|___|___" << endl;
        cout << "   |   |   " << endl;
        cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << " " << endl;
        cout << "   |   |   " << endl;

        if (winner != ' ') {
            break;
        }

        cout << "Player " << currentPlayer << "'s Turn." << endl;

        while(true) {
            cout << "Enter the row and column #'s (1-3): ";
            cin >> r >> c;
            if (r < 1 || r > 3 || c < 1 || c > 3) {
                cout << "Invalid Input Please Try Again.\n";
            }
            else if(board[r-1][c-1] != ' ') {
                cout << "Tile is full, try again.\n";
            }
            else {
                break;
            }
            cin.clear(); // clear error flags (in case user enters letters in input)
            cin.ignore(10000, '\n'); // discard values, skips to the next new line char
        }

        board[r - 1][c - 1] = currentPlayer;
        currentPlayer = (currentPlayer == playerX) ? playerO : playerX;

        //check winner - rows - horizontal
        for (int r = 0; r < 3; r++) {
            if (board[r][0] != ' ' && board[r][0] == board[r][1] && board[r][1] == board[r][2]) {
                winner = board[r][0];
                break;
            }
        }

        // check winner - rows - vertical
        for (int c = 0; c <3; c++) {
            if (board[0][c] != ' ' && board[0][c] == board[1][c] && board[1][c] == board[2][c]) {
                winner = board[0][c];
                break;
            }
        }

        if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            winner = board[0][0];
        }
        else if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            winner = board[0][2];
        }
    }

    if (winner != ' ') {
        cout << "Player " << winner << " is the winner!" << endl;
    }
    else {
        cout << "No winner! The game was a tie!" << endl;
    }
}