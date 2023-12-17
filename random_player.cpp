#include "random_player.h"
#include <cstdlib>


int RandomPlayer::decideOnBoard(State board) {
  int movesCount = board.possiblepieceboards.size();
  return rand() % movesCount;
}

