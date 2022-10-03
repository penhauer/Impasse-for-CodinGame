#pragma once

class Ai
{
private:
    int color;
    Move bestMove;

public:
    Ai();
    Ai(int color);
    Move getMove(Board board);

private:
    Move randomMove(const Board &board) const;
    void orderMoves();
    float negamax(Board board, int depth, int color);
};