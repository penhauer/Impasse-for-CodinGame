#pragma once
#include <algorithm>
#include <tuple>
#include "board.h"
#include "ai.h"

class Game
{
private:
    int player; // 1 = White vs Ai, -1 = Black vs Ai
    int gui;    // 0 = no gui, 1 = gui
    Board board;
    Ai ai;

public:
    Game();
    ~Game();

private:
    void gameLoop();
    std::tuple<bool, Move> trySelect(int row, int col, Move move);
    void reset();
    int parseMove(const std::string &notation);
    std::tuple<bool, Move> returnIfOnlyMove(const Move &move);
};