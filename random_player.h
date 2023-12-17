#pragma once

#include "player.h"


class RandomPlayer : public Player {

  public:
    int decideOnBoard(State state);
};


