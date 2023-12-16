#pragma once
#include "common.h"

typedef std::set<Pos> PosSet;


struct PieceCount {
    int whiteSingles = 0;
    int whiteDoubles = 0;
    int blackSingles = 0;
    int blackDoubles = 0;
};

class PieceBoard {
public:
    PieceCount piececount;

    std::map<int, int> transitions = {{WHITE, 0}, {BLACK, 0}};
    std::map<int, int> distances = {{WHITE, 26}, {BLACK, 26}};
    std::map<int, Pos> postocrown;

    Move lastmove;
    PieceBoard();
    int evaluate(int color) const;


  Piece table[ROWS][COLS]; 


  int count() {
    return piececount.blackDoubles + piececount.blackSingles + piececount.whiteSingles + piececount.whiteDoubles;
  }

  Piece getPiece(Pos pos);
  void setPiece(Pos pos, Piece piece);
  void removePiece(Pos pos);
  bool isEmpty(Pos pos);

  void printBoard();
};



const int BOARD_GAME_ONGOING = -10;
const int BOARD_WHITE_WON = WHITE;
const int BOARD_BLACK_WON = BLACK;


class Board {
public:
    int turn;
    int winner;

    std::vector<PieceBoard>  possiblepieceboards;
    std::vector<PieceBoard>  pieceboardhistory;
    PieceBoard pieceboard;

    Board();
    void printBoard();
    void generateLegalMoves();
    void doMove(int moveNumber);
    void doMove(PieceBoard nextPieceBoard);
    void undoMove();



private:
    void move(PieceBoard &pieceboard, Pos pos, Pos toPos);
    bool crownIf(PieceBoard &pieceboard, Pos pos);
    void crown(PieceBoard &pieceboard, Pos pos);
    void bearOff(PieceBoard &pieceboard, Pos pos);
    void remove(PieceBoard &pieceboard, Pos pos);
    PosSet checkSingles(Pos pos);
    void addPieceMoves(Pos pos);
    void addImpassable();
    void newBoard();
    bool isTransposable(Pos pos, Pos toPos);
    void checkImpasseForPos(Pos pos);
    void changeTurn();

    bool inside(Pos pos);
};
