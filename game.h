#pragma once

#include "ai.h"
#include "player.h"
#include "common.h"


class Game {

  public:
    int turns = 0;

    Game();
    int gameLoop(Player *white, Player *black);

  private:
    Board board;
    std::tuple<bool, PieceBoard> trySelect(Pos pos, PieceBoard pb);
    std::tuple<int, int> timer;

    void undoPlayerMove();
};
