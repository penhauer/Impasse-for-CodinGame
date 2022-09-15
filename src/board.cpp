#include "piece.h";
#include "board.h";

class GameState
{
private:
    int turn; // 1 = white, -1 = black
    int gameState; // 0 = in progress, 1 = white win, -1 = black win
    int whiteSingles; //number of white singles
    int whiteDoubles; //number of white doubles
    int blackSingles; //number of black singles
    int blackDoubles; //number of black doubles
    Piece board[8][8];
    void newGame() {
        turn = 1;
        gameState = 0;
        whiteSingles = 6;
        whiteDoubles = 6;
        blackSingles = 6;
        blackDoubles = 6;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // 1: (0,0), (3,1), (4,0), (1,7)
                // 2: (1,7), (2,6), (5,7), (6,6)
                // -1: (0,6), (3,7), (4,6), (7,7)
                // -2: (1,1), (2,0), (5,1), (6,0)
                if ((j < 2) & ((i+j) % 4)) {
                    board[i][j] = Piece(1, i, j, false);
                } else if ((j > 5) & ((i+j) % 4 == 0)) {
                    board[i][j] = Piece(1, i, j, true);
                } else  if ((j > 5) & ((i+j) % 4 == 2)) {
                    board[i][j] = Piece(-1, i, j, false);
                } else if ((j < 2) & ((i+j) % 4 == 2)) {
                    board[i][j] = Piece(-1, i, j, true);
                //} else {
                //    board[i][j] = NULL;
                };
            };
        };
    }
public:
    GameState()
    {
        newGame()
    };
    void getMoves(int color) {
        // return array of possible moves
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (Piece piece = board[i][j]; piece.getColor() == color) {
                    moves = checkDiagonals(i,j);
                    // add moves to array
                    };
                };
            };
        };
    void checkDiagonals(int x, int y, bool forward) {
        // check diagonals forward
        int j = y;
        while ((j > 0) & (j < 8)) {
            int j = y + 1;
            if (forward) {
                for (int i = x + 1; i < 8; i++) {
                    if (board[i][j] == 0) {
                        // add to possible moves
                    } else {
                        break;
                    }
                }
                // code here
                j = j + 1;
            } else {
                // code here
                j = j - 1;
            };
        };
    };
    void movePiece(int x, int y, int newX, int newY) {
        board[newX][newY] = board[x][y];
        board[x][y] = 0;
        turn = turn * -1;
    };
};