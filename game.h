#pragma once

#include "ai.h"
#include "ai_agent.h"
#include "common.h"

class Game {
public:
    Board board;

private:
    int timemin;
    Ai ai;
    std::tuple<int, int> timer;

public:
    Game(int timemin);
    Game();
    int gameLoop(Player *white, Player *black);

private:
    std::tuple<bool, PieceBoard> trySelect(Pos pos, PieceBoard pb);
    void undoPlayerMove();
};
