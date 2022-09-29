#pragma once
#include <map>
#include <set>
#include <vector>

typedef int BoardArray[64];

typedef std::set<int> PosSet;

struct Piece
{
    int piece;
    int pos;
    Piece();
    Piece(int piece, int pos);
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
        return from.pos < other.from.pos && to.pos < other.to.pos && remove.pos < other.remove.pos;
    };
};

typedef std::set<Move> MoveSet;

typedef std::map<int, int> PieceToCrown;

struct PieceCount
{
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
};

typedef std::vector<BoardArray> BoardHistory;

class Board
{
public:
    int turn;
    int state;
    BoardArray boardarray;
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
    void updateMoveSet();
    int pieceDirection(const int &piece) const;
    void doMove(const Move &move);
    void undoMove();

private:
    void changePieceType(const Piece &p);
    void removePiece(const Piece &p);
    PosSet checkSingles() const;
    void addPieceDiagonals(const int &pos);
    void crownIf(const Piece &p);
    void addImpassable();
    void initBoard(bool paused);
    void getPieceCount();
    void getIfToCrown();
};