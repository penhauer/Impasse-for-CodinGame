#pragma once
#include "common.h"
#include <vector>



struct PieceCount {
  short whiteSingles = 0;
  short whiteDoubles = 0;
  short blackSingles = 0;
  short blackDoubles = 0;
};

class PieceBoard {
  public:
    short winner;
    PieceCount piececount;
    Piece table[ROWS * COLS / 2]; 

    Pos posToCrown[2] = {EMPTY_POSE, EMPTY_POSE};

    Move lastmove;
    PieceBoard();


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


