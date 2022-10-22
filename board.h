#pragma once
#include "common.h"

typedef std::set<int> PosSet;

typedef std::map<int, Piece> PieceMap;

typedef std::map<int, int> PosToCrown;

struct PieceCount
{
    int whiteSingles = 0;
    int whiteDoubles = 0;
    int blackSingles = 0;
    int blackDoubles = 0;
};

class PieceBoard
{
public:
    PieceCount piececount;
    PieceMap piecemap;
    PosToCrown postocrown;
    Move lastmove;
    PieceBoard();
    int evaluate(int color) const;
};

typedef std::vector<PieceBoard> PieceBoardVector;

class Board
{
public:
    int turn;
    int state;
    PieceBoardVector possiblepieceboards;
    PieceBoardVector pieceboardhistory;
    PieceBoard pieceboard;

    Board();
    Board(bool paused);
    void reset();
    void reset(Board oldboard);
    void initBoard();
    void printBoard() const;
    void generateLegalMoves();
    void doMove(const PieceBoard new_pieceboard);
    void undoMove();

private:
    void move(PieceBoard &pieceboard, int pos, int topos);
    bool crownIf(PieceBoard &pieceboard, const Piece &p, const int &pos);
    void crown(PieceBoard &pieceboard, const Piece &p, const int &pos);
    void bearOff(PieceBoard &pieceboard, const Piece &p, const int &pos);
    void remove(PieceBoard &pieceboard, const Piece &p, const int &pos);
    PosSet checkSingles(const Piece &piece, const int &pos) const;
    void addPieceMoves(const Piece &piece, const int &pos);
    void addImpassable();
    void newBoard();
    void getPieceCount();
    bool ifTransposable(const Piece &piece, const int &piecerow, const int &i, const int &topos) const;
    bool ifEmptySquare(const int &pos) const;
};
