#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"


class Player {

public:
  virtual int decideOnBoard(State board) = 0;
};


#endif
