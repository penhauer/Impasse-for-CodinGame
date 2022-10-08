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

struct PieceCount
{
    int whiteSingles = 0;
    int whiteDoubles = 0;
    int blackSingles = 0;
    int blackDoubles = 0;
};

struct PieceBoard
{
    PieceCount piececount;
    PieceMap piecemap;
    PieceToCrown piecetocrown;
    Move lastmove;
};

typedef std::vector<PieceBoard> PieceBoardVector;

class Board
{
public:
    int turn;
    int state;
    PieceBoardVector possibleboards;

private:
    PieceBoard pieceboard;

public:
    Board();
    Board(bool paused);
    void resetBoard(bool paused);
    void restoreLast();
    void saveBoard() const;
    void loadBoard(int turn, PieceToCrown piecetocrown, PieceSet pieceset);
    void deleteBoard() const;
    void printBoard() const;
    void printMove(const Move &move) const;
    void createPossibleBoards();
    void move(PieceBoard &pieceboard, Piece piece, Piece square);
    void doMove(const PieceBoard &new_pieceboard);
    void undoMove();
    float evaluate() const;

private:
    void updateMoveSet(const Move &move);
    bool crownIf(PieceBoard &pieceboard, const Piece &p);
    void crown(PieceBoard &pieceboard, const Piece &p);
    void bearOff(PieceBoard &pieceboard, const Piece &p);
    void remove(PieceBoard &pieceboard, const Piece &p);
    PieceSet checkSingles(PieceBoard &pieceboard, const Piece &piece) const;
    void addPieceMoves(const Piece &piece);
    void addImpassable();
    void initBoard();
    void getPieceCount();
    bool ifTransposable(const Piece &piece, const int &i, const int &row, const int &col) const;
    bool ifEmptySquare(const Piece &piece, const int &row, const int &col) const;
};
