#include "random_player.h"
#include <cstdlib>


int RandomPlayer::decideOnBoard(State state) {
  int movesCount = state.possiblepieceboards.size();
  return rand() % movesCount;
}

