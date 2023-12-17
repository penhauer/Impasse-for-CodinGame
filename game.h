#pragma once

#include "ai.h"
#include "player.h"
#include "common.h"


class Game {

  public:
    int turns = 0;
    Player *white, *black;
    Game(Player *white, Player *black);
    int gameLoop();

  private:
    State board;
    std::tuple<bool, PieceBoard> trySelect(Pos pos, PieceBoard pb);
    std::tuple<int, int> timer;

    void undoPlayerMove();
};
