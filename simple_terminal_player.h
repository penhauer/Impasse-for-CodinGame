#pragma once

#include "player.h"
#include "common.h"
#include <iostream>


class SimpleTerminalPlayer: public Player {

  public:
    int decideOnBoard(Board board) override;
};
