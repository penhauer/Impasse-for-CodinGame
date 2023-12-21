#pragma once

#include "player.h"
#include "common.h"


class Game {

  public:
    bool print = false;
    int turns = 0;
    Player *white, *black;
    Game();
    Game(State *state) {
      this->state = *state;
    };
    int gameLoop(Player *white, Player *black);
    void codingGameLoop(Player *us);

  private:
    State state;
    std::tuple<bool, PieceBoard> trySelect(Pos pos, PieceBoard pb);
    std::tuple<int, int> timer;
    void undoPlayerMove();
};
