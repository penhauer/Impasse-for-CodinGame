#pragma once
#include "board.h"
#include <map>
#include <unordered_map>
#include <set>
#include <vector>

typedef std::set<int> PosSet;
// Map of all possible moves at the moment

struct Move
{
    Piece from;
    Piece to;
    Piece remove;
    Move(Piece from, Piece to, Piece remove = Piece(0,-1)) : from(from), to(to), remove(remove){};

    bool operator<(const Move &other) const
    {
        return from.pos < other.from.pos && to.pos < other.to.pos && remove.pos < other.remove.pos;
    };
};

typedef std::set<Move> MoveSet;

typedef std::unordered_map<int, MoveSet> MoveMap;

struct PosMoveSet
{
    int pos;
    MoveSet moveset;
    PosMoveSet(int pos, MoveSet moveset) : pos(pos), moveset(moveset){};
};

typedef std::vector<Move> MoveStack;

typedef std::map<int,int> ToCrown;

struct Piece
{
    int piece;
    int pos;
    Piece(int piece, int pos) : piece(piece), pos(pos){};
};

class GameState
{
public:
    int player;
    int turn;
    int state;
    PieceCount pieces;
    Board board;
    MoveMap movemap;
    MoveStack movestack;
    ToCrown tocrown;
    void reset();
    int pieceDirection(const int &piece) const;
    void doMove(const Move &move);
    void removePiece(const Piece &p);
    void changePieceType(const Piece &p);
    PosMoveSet checkPieceDiagonals(const int &pos) const;
    void makeMoveMap();
    void crownIf(const Piece &p);
    PosMoveSet addImpassable();
    PosSet checkSingles() const;
    void doMove(const Move &move);
    void undoMove();
};