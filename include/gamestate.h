#pragma once

#include <unordered_map>
#include <set>

struct Piece;
struct Move;
typedef PieceMap;
typedef MoveMap;
typedef Board;

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
    PieceMap piecemap;
    void newGame();
public:
    int getGameState();
    int getTurn();
    int getDirection(int color, int isDouble);
    void movePiece(Move move);
    void removePiece(int col, int row);
    void changePieceType(int col, int row);
    MoveMap checkDiagonals(int col, int row, bool forward);
    void makeMove(Move move);
    MoveMap getMoves(int color);
};