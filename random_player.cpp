#include "random_player.h"
#include <cstdlib>


int RandomPlayer::decideOnBoard(State state, std::string lastMove) {
  int movesCount = state.possiblepieceboards.size();
  return rand() % movesCount;
}

