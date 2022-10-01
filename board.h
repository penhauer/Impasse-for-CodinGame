#pragma once
#include <map>
#include <unordered_map>
#include <set>
#include <vector>

typedef std::set<Piece> PieceSet;

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
};

struct Move
{
    Piece from;
    Piece to;
    Piece remove;
    Move();
    Move(Piece from, Piece to, Piece remove);
    // valid if from.pos exist
    bool validMove() const;
    bool operator<(const Move &other) const
    {
        return from.col < other.from.col && from.row < other.from.row && to.col < other.to.col && to.row < other.to.row && remove.col < other.remove.col && remove.row < other.remove.row;
    };
};

typedef std::set<Move> MoveSet;

typedef std::map<int, Piece> PieceToCrown;

struct PieceCount
{
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
};

typedef std::vector<PieceMap> BoardHistory;

typedef std::unordered_map<int, std::unordered_map<int, Piece>> PieceMap;

class Board
{
public:
    int turn;
    int state;
    PieceMap piecemap;
    PieceCount piececount;
    MoveSet moveset;
    bool boolPieceToCrown;

private:
    BoardHistory *boardhistory;
    PieceToCrown piecetocrown;

public:
    Board();
    Board(bool paused);
    void resetBoard(bool paused);
    void restoreLast(const Board &b);
    void saveBoard() const;
    void deleteBoard() const;
    void printBoard() const;
    void printMoves() const;
    void createMoveSet();
    void updateMoveSet(const Move &move);
    void doMove(const Move &move);
    void undoMove();

private:
    void crown(const Piece &p);
    void bearOff(const Piece &p);
    void remove(const Piece &p);
    PieceSet checkSingles(const Piece &piece) const;
    void addPieceDiagonals(const Piece &p);
    void crownIf(const Piece &p);
    void addImpassable();
    void initBoard(bool paused);
    void getPieceCount();
    void getIfToCrown();
};