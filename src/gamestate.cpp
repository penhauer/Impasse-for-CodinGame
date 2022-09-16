#include "../include/piece.h"
#include <map>
#include <set>

typedef std::pair <int, int> Move;
typedef std::map <int, Piece> PieceMap;
typedef std::map <int, std::set <int>> MoveMap;

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
    PieceMap piecemap;
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
                    piecemap[i+j*8] = Piece(1, i, j, false);
                } else if ((j > 5) & ((i+j) % 4 == 0)) {
                    board[i][j] = 2;
                    piecemap[i+j*8] = Piece(1, i, j, true);
                } else  if ((j > 5) & ((i+j) % 4 == 2)) {
                    board[i][j] = -1;
                    piecemap[i+j*8] = Piece(-1, i, j, false);
                } else if ((j < 2) & ((i+j) % 4 == 2)) {
                    board[i][j] = -2;
                    piecemap[i+j*8] = Piece(-1, i, j, true);
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
    void makeMove(Move move) {
        MoveMap moves = getMoves(turn);
        // TODO hashmap instead of set for MoveMap with type of move and maybe already the reward
        if (moves.count(move.first) & moves[move.first].count(move.second)) {
            // if move is delete piece ...
            // if move is king piece ...
            // if move is impasse ...
            movePiece(move);
        }
            
        turn = turn * -1;
    };
    MoveMap getMoves(int color) {
        // return array of possible moves
        MoveMap moves;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece piece = piecemap[i+j*8];
                if (piece.getColor() == color) {
                    int direction = getDirection(piece.getColor(), piece.getType());
                    MoveMap new_moves = checkDiagonals(i, j, direction);
                    moves.insert(new_moves.begin(), new_moves.end());
                    };
                };
            };
        return moves;
        };
    int getDirection(int color, int isDouble) {
        if ((color == 1 & !isDouble) | (color == -1 & isDouble)) {
                return 1;
            } else {
                return -1;
            };
        };
    MoveMap checkDiagonals(int x, int y, bool forward) {
        // check diagonals forward
        int j = y;
        MoveMap movemap;
        while ((j > 0) & (j < 8)) {
            int j = y + 1;
            if (forward) {
                for (int i = x + 1; i < 8; i++) {
                    if (board[i][j] == 0) {
                        movemap[x+y*8].insert(i, j);
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
        return movemap;
    };
    void movePiece(Move move) {
        int x = move.first % 8;
        int y = move.first - x;
        int newX = move.second % 8;
        int newY = move.second - newX;
        board[newX][newY] = board[x][y];
        board[x][y] = 0;
        piecemap[move.second] = piecemap[move.first];
        piecemap.erase(x+y*8);
    };
    void removePiece(int x, int y) {
        board[x][y] = 0;
        piecemap.erase(x+y*8);
    };
    void changePieceType(int x, int y) {
        if (piecemap[x+y*8].getType() == true) {
            piecemap[x+y*8].setType(false);
        } else {
            piecemap[x+y*8].setType(true);
        };
    };

};