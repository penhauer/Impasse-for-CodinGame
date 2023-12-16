#ifndef AGENT_H
#define AGENT_H

#include "board.h"


class Player {

public:
  virtual void decideOnBoard(Board &board) {
    std::cout << "oh oh " << std::endl;
  }
};

class TerminalPlayer: public Player {

public:
  void decideOnBoard(Board &board) override;

private:
  std::tuple<bool, PieceBoard> trySelect(Board board, Pos pos, PieceBoard pb);
  void undoPlayerMove(Board &board);
};


class AIPlayer: public Player {

  public:
    void decideOnBoard(Board &board);
};

#endif
