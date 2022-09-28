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
    Move(Piece remove = Piece(0, -1));
    Move(Piece from, Piece to, Piece remove = Piece(0,-1));
    bool validMove();
    bool operator<(const Move &other) const;
};

typedef std::set<Move> MoveSet;

typedef std::map<int,int> PieceToCrown;

struct PieceCount {
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
    BoardHistory *boardhistory;
    PieceToCrown piecetocrown;
    Board();
    Board(bool paused);
    void resetBoard(bool paused);
    void restoreLast(const Board &b);
    int pieceDirection(const int &piece) const;
    void removePiece(const Piece &p);
    void changePieceType(const Piece &p);
    void addPieceDiagonals(const int &pos);
    void updateMoveSet();
    void crownIf(const Piece &p);
    void addImpassable();
    PosSet checkSingles() const;
    void doMove(const Move &move);
    void undoMove();
    void printMoves() const;
    void printBoard() const;
    void saveBoard() const;
    void deleteBoard() const;
private:
    void initBoard(bool paused);
    void getPieceCount();
};