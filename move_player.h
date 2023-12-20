#pragma once

#include "player.h"


class MovePlayer: public Player {
  private:

  public:
    MovePlayer(int color) : Player(color) {};
    int decideOnBoard(State state) override;
};
