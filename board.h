#pragma once
#include <map>
#include <set>
#include <vector>

typedef int BoardArray[64];

typedef std::set<Piece> PieceSet;

class Piece
{
public:
    int piece;
    int sol;
    int bsol;
    int pos;
    int direction;
    void changeType();
    void changePos(int sol, int bsol);
    Piece();
    Piece(int piece, int sol, int bsol);
private:
    void getPos();
    void getDirection();
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

typedef std::map<int, Piece> PieceToCrown;

struct PieceCount
{
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
};

typedef std::vector<BoardArray> BoardHistory;

typedef std::map<int, std::map<int, Piece>> PieceMap;
class Board
{
public:
    int turn;
    int state;
    BoardArray boardarray;
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
    int pieceDirection(const int &piece) const;
    void doMove(const Move &move);
    void undoMove();

private:
    void changePieceType(const Piece &p);
    void removePiece(const Piece &p);
    PieceSet Board::checkSingles() const;
    void Board::addPieceDiagonals(const Piece &p);
    void updateMoveSet(const Move &move);
    void crownIf(const Piece &p);
    void addImpassable();
    void initBoard(bool paused);
    void getPieceCount();
    void getIfToCrown();
};