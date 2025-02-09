#ifndef PLAYER_H
#define PLAYER_H

#include "state.h"


class Player {
  private:

  public:
    int color;

    Player(int color) {
      this->color = color;
    }

    virtual int decideOnBoard(State state, std::string lastMove) = 0;
};


#endif
