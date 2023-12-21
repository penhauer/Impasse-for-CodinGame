#pragma once

#include "player.h"
#include "common.h"
#include <iostream>


class SimpleTerminalPlayer: public Player {

  public:
    SimpleTerminalPlayer(int color) : Player(color){};
    int decideOnBoard(State state, std::string lastMove) override;
};
