#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"


class Player {

public:
  virtual int decideOnBoard(Board board) = 0;
};


#endif
