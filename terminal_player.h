#pragma once

#include "player.h"
#include <iostream>

class TerminalPlayer: public Player {

public:
  void decideOnBoard(Board &board) override;

private:
  std::tuple<bool, PieceBoard> trySelect(Board board, Pos pos, PieceBoard pb);
  void undoPlayerMove(Board &board);
};
