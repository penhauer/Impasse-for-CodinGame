#include "move_player.h"


int MovePlayer::decideOnBoard(State state) {
  std::string move;
  std::cin >> move;

  Pos pos = Pos::parseMove(move.substr(0, 2));
  Pos to = EMPTY_POSE;
  Pos remove = EMPTY_POSE;
  if (move.size() > 2)
    to = Pos::parseMove(move.substr(2, 2));
  if (move.size() > 4)
    remove = Pos::parseMove(move.substr(4, 2));
  
  for (int i = 0; i < state.possiblepieceboards.size(); i++) {
    auto child = state.possiblepieceboards[i];
    auto cm = child.lastmove;
    if (!(cm.from == pos))
      continue;
    if (remove == EMPTY_POSE && to == EMPTY_POSE)
      return i;

    if (!(remove == EMPTY_POSE) && (to == cm.to) && (remove == cm.remove))
      return i;
    if (cm.to == to)
      return i;

    if (cm.to == EMPTY_POSE && cm.remove == to)
      return i;
  }
  return -1;
}

