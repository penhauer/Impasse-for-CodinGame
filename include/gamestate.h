#pragma once

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
    void newGame();
public:
    void makeMove(Move move);
    MoveMap getMoves(int color);
    int getDirection(int color, int isDouble);
    MoveMap checkDiagonals(int x, int y, bool forward);
    void movePiece(Move move);
    void removePiece(int x, int y);
    void changePieceType(int x, int y);
};