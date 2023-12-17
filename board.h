#pragma once
#include "common.h"
#include <vector>



struct PieceCount {
  int whiteSingles = 0;
  int whiteDoubles = 0;
  int blackSingles = 0;
  int blackDoubles = 0;
};

class PieceBoard {
  public:
    int winner;
    PieceCount piececount;

    int transitions[2] = {0, 0};
    int distances[2] = {26, 26};
    Pos posToCrown[2] = {EMPTY_POSE, EMPTY_POSE};

    Move lastmove;
    PieceBoard();
    int evaluate(int color) const;


    Piece table[ROWS * COLS / 2]; 


    void placePieces();

    Piece getPiece(Pos pos);
    void setPiece(Pos pos, Piece piece);
    void removePiece(Pos pos);
    bool isEmpty(Pos pos);

    void printBoard();
    void doSanityCheck();

    void move(Pos pos, Pos toPos);
    Pos crownIf(Pos pos);
    void crown(Pos pos);
    void bearOff(Pos pos);
    void remove(Pos pos);
};



const int BOARD_GAME_ONGOING = -10;
const int BOARD_WHITE_WON = WHITE;
const int BOARD_BLACK_WON = BLACK;


class State {
  public:
    int turn;

    std::vector<PieceBoard>  possiblepieceboards;
    std::vector<PieceBoard>  pieceboardhistory;
    PieceBoard pieceboard;

    State();
    State(PieceBoard pb);

    void printBoard();
    void generateLegalMoves();
    void doMove(int moveNumber);
    void doMove(PieceBoard nextPieceBoard);
    void undoMove();

    State newChildState();



  private:

    std::vector<Pos> checkSingles(Pos pos);
    void addPieceMoves(Pos pos);
    void addImpassable();
    void changeTurn();

    bool isTransposable(Pos pos, Pos toPos);
    void checkImpasseForPos(Pos pos);

    void checkBearOff(Pos pos, Pos toPos);
    void checkCrown(Pos pos, Pos toPos);
    void doSimpleMove(Pos pos, Pos toPos);

    bool inside(Pos pos);
};
