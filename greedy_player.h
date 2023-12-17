#pragma once

#include "player.h"


class GreedyPlayer: public Player {

  public:
    GreedyPlayer();
    int decideOnBoard(State state) override;
};
