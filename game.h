#pragma once
#include <algorithm>
#include <tuple>
#include <vector>
#include "board.h"
#include "ai.h"
#include "common.h"

class Game
{
public:
    Board board;

private:
    int player; // 1 = White vs Ai, -1 = Black vs Ai
    int timemin;
    Ai ai;
    std::tuple<int, int> timer;

public:
    Game(int player, int timemin);

private:
    void gameLoop();
    void undoMove();
    void reset(int player, int timemin);
    std::tuple<bool, PieceBoard> trySelect(int pos, PieceBoard pb);
    std::tuple<bool, PieceBoard> returnIfOnlyBoard(const PieceBoard &pb);
};