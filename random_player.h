#pragma once

#include "player.h"


class RandomPlayer : public Player {

  public:
    RandomPlayer(int color) : Player(color) {};

    int decideOnBoard(State state);
};


