#pragma once

#include "player.h"
#include "common.h"


class TerminalReaderPlayer: public Player {
  private:
    Player *player;

  public:
    TerminalReaderPlayer(Player *player) : Player(player->color), player(player){};
    int decideOnBoard(State state) override;
};
