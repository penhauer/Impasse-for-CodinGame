#include "../include/piece.h"
#include "../include/board.h"
#include <map>
#include <vector>
using namespace std;

struct Move {
    int x;
    int y;
    int newX;
    int newY;
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
    int board[8][8];
    map <int, Piece> pieces;
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
                    board[i][j] = 1;
                    pieces[i*8+j] = Piece(1, i, j, false);
                } else if ((j > 5) & ((i+j) % 4 == 0)) {
                    board[i][j] = 2;
                    pieces[i*8+j] = Piece(1, i, j, true);
                } else  if ((j > 5) & ((i+j) % 4 == 2)) {
                    board[i][j] = -1;
                    pieces[i*8+j] = Piece(-1, i, j, false);
                } else if ((j < 2) & ((i+j) % 4 == 2)) {
                    board[i][j] = -2;
                    pieces[i*8+j] = Piece(-1, i, j, true);
                } else {
                    board[i][j] = 0;
                };
            };
        };
    };
public:
    GameState()
    {
        newGame();
    };
    void getMoves(int color) {
        // return array of possible moves
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece piece = board[i][j];
                if (piece.getColor() == color) {
                    int direction = getDirection(piece.getColor(), piece.getType());
                    vector<Move> moves = checkDiagonals(i, j, direction);
                    // add moves to array
                    } else {
                        // do nothing
                    };
                };
            };
        };
    int getDirection(int color, int isDouble) {
        if ((color == 1 & !isDouble) | (color == -1 & isDouble)) {
                return 1;
            } else {
                return -1;
            };
        };
    vector<Move> checkDiagonals(int x, int y, bool forward) {
        // check diagonals forward
        int j = y;
        vector<Move> moves;
        while ((j > 0) & (j < 8)) {
            int j = y + 1;
            if (forward) {
                for (int i = x + 1; i < 8; i++) {
                    if (board[i][j] == 0) {
                        moves.push_back(Move{x, y, i, j});
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
        return moves;
    };
    void movePiece(int x, int y, int newX, int newY) {
        board[newX][newY] = board[x][y];
        pieces[newX*8+newY] = pieces[x*8+y];
        pieces.erase(x*8+y);
        board[x][y] = 0;
        turn = turn * -1;
    };
};