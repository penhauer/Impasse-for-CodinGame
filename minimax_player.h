#pragma once

#include "player.h"


struct MinMaxValIndex {
  MinMaxValIndex(int value, int index) {
    this->value = value;
    this->index = index;
  }
  int value;
  int index;
};

class MiniMaxPlayer: public Player {
  private:
    int maxDepth;
    int evaluateBoard(PieceBoard &pieceBoard);
    MinMaxValIndex minimax(int depth, State *state, int alpha, int beta, bool maximizing);

  public:
    MiniMaxPlayer(int color, int maxDepth) : Player(color), maxDepth(maxDepth) {};
    int decideOnBoard(State state) override;
};
