#pragma once
#include "board.h"

struct Piece;
struct Move;
typedef MoveSet;
typedef MoveMap;
typedef PosMoveSet;

class GameState
{
public:
    int player;
    int turn;
    int state;
    PieceCount pieces;
    Board board;
    MoveMap movemap;
    int pieceDirection(const int &piece) const;
    void movePiece(const Move &move);
    void removePiece(const Pos &pos);
    void changePieceType(const Move &move);
    PosMoveSet checkPieceDiagonals(const int &col, const int &row);
    void makeMoveMap();
    PosMoveSet addImpassable();
    void doMove(const Move &move);
};