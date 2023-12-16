#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"


class Player {

public:
  virtual void decideOnBoard(Board &board) {
    std::cout << "oh oh " << std::endl;
  }
};


#endif
