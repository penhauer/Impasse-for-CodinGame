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
    Piece table[ROWS * COLS / 2]; 

    int transitions[2] = {0, 0};
    int distances[2] = {26, 26};
    Pos posToCrown[2] = {EMPTY_POSE, EMPTY_POSE};

    Move lastmove;
    PieceBoard();
    int evaluate(int color) const;




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


