#pragma once
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <math.h>
#include "common.h"

class Piece
{
public:
    int piece;
    int color;
    int col;
    int row;
    int direction;
    Piece();
    Piece(int piece, int color, int col, int row);
    void getDirection();
    void changeType();
    bool operator<(const Piece &other) const;
    bool operator==(const Piece &other) const;
};

struct Move
{
    Piece from;
    Piece to;
    Piece remove;
    Move();
    Move(Piece from, Piece to, Piece remove);
    bool operator<(const Move &other) const;
    bool operator==(const Move &other) const;
};

typedef std::set<Piece> PieceSet;

typedef std::map<int, std::map<int, Piece>> PieceMap;

typedef std::map<int, Piece> PieceToCrown;

typedef std::set<Move> MoveSet;

struct PieceCount
{
    int whiteSingles = 0;
    int whiteDoubles = 0;
    int blackSingles = 0;
    int blackDoubles = 0;
};

class Board
{
public:
    int turn;
    int state;
    PieceCount piececount;
    MoveSet moveset;

private:
    PieceMap piecemap;
    bool boolPieceToCrown;
    PieceToCrown piecetocrown;

public:
    Board();
    Board(bool paused);
    void resetBoard(bool paused);
    void restoreLast();
    void saveBoard() const;
    void loadBoard();
    void deleteBoard() const;
    void printBoard() const;
    void printMoves() const;
    void createMoveSet();
    void doMove(const Move &move);
    float evaluate() const;

private:
    void updateMoveSet(const Move &move);
    void crown(const Piece &p);
    void bearOff(const Piece &p);
    void remove(const Piece &p);
    PieceSet checkSingles(const Piece &piece) const;
    void addPieceDiagonals(const Piece &p);
    void crownIf(const Piece &p);
    void addImpassable();
    void initBoard();
    void getPieceCount();
    bool ifTransposable(const Piece &piece, const int &i, const int &row, const int &col) const;
    bool ifEmptySquare(const Piece &piece, const int &row, const int &col) const;
};
