#pragma once

typedef Board;
struct Pos
{
    int col;
    int row;
};
struct Piece
{
    int piece;
    Pos pos;
};
struct Move
{
    int piece;
    Pos from;
    Pos to;
    Pos remove;
};
typedef MoveSet;
typedef MoveMap;
typedef MoveSetPair;
struct Pieces
{
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
};

class GameState
{
public:
    int player;
    int turn;
    int state;
    Pieces pieces;
    Board board;
    MoveMap movemap;
    int pieceDirection(const int &piece) const;
    void movePiece(const Move &move);
    void removePiece(const Pos &pos);
    void changePieceType(const Move &move);
    MoveSetPair checkPieceDiagonals(const int &col, const int &row);
    void makeMoveMap();
    MoveSetPair checkDeleteable();
    void makeMove(const Move &move);
};