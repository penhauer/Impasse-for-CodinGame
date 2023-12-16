#include "random_player.h"
#include <cstdlib>


int RandomPlayer::decideOnBoard(Board board) {
  int movesCount = board.possiblepieceboards.size();
  return rand() % movesCount;
}

