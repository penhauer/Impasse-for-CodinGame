#pragma once

#include "player.h"


class GreedyPlayer: public Player {
  private:
    int evaluateBoard(PieceBoard &pieceBoard);

  public:
    GreedyPlayer(int color) : Player(color) {};
    int decideOnBoard(State state) override;
};
