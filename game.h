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
    Ai ai;

public:
    Game(int player);
    ~Game();

private:
    void gameLoop();
    void undoMove();
    void reset();
    std::tuple<bool, PieceBoard> trySelect(int pos, PieceBoard pb);
    std::tuple<bool, PieceBoard> returnIfOnlyBoard(const PieceBoard &pb);
};