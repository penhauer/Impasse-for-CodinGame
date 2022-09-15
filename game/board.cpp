#include "common.h";

struct Board
{
    // 8x8 grid with Pieces on them
    Piece[8][8] board;
    // constructor

};

{
private:
    int board[8][8];
};

class GameState
{
private:
    int turn; // 1 = white, -1 = black
    int gameState; // 0 = in progress, 1 = white win, -1 = black win
    int whiteSingles; //number of white singles
    int whiteDoubles; //number of white doubles
    int blackSingles; //number of black singles
    int blackDoubles; //number of black doubles
    Board board;
public:
    GameState()
    {
        turn = 1;
        gameState = 0;
        whiteSingles = 6;
        whiteDoubles = 6;
        blackSingles = 6;
        blackDoubles = 6;
        board = create_board();
    };

    int[8][8] create_board() {
        board = new int[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // 1: (0,0), (1,3), (0,4), (1,7)
                // 2: (7,1), (6,2), (7,5), (6,6)
                // -1: (6,0), (7,3), (6,4), (7,7)
                // -2: (1,1), (0,2), (1,5), (0,6)
                if (j < 2) and ((i+j) %% 4) {
                    board[i][j] = Piece();
                } else if (j > 5) and ((i+j) %% 4 == 0) {
                    board[i][j] = 2;
                } else  if (j > 5) and ((i+j) %% 4 == 2) {
                    board[i][j] = -1;
                } else if (j < 2) and ((i+j) %% 4 == 2) {
                    board[i][j] = -2;
                } else {
                    board[i][j] = 0;
                };
            };
        };
        return board;
    };
    
    void getMoves(int turn) {
        // return array of possible moves
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (Piece piece = board[i][j]; piece.getColor() == turn) {
                    moves = checkDiagonals(i,j);
                    // add moves to array
                };
                    
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