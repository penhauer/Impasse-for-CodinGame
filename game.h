#pragma once
#include "ai.h"

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
    Game();
    Game(int player, int timemin);

private:
    void reset(bool cont);
    void gameLoop();
    std::tuple<bool, PieceBoard> trySelect(int pos, PieceBoard pb);
    void undoPlayerMove();
    void save() const;
    void restoreSave();
    void deleteSave() const;
};