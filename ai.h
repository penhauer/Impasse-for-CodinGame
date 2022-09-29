#pragma once

class Ai
{
private:
    int color;
    Move bestMove;
public:
    Ai();
    Ai(int color);
    Move getMove(const Board& board);
private:
    int evaluateBoard(Board board) const;
    Move randomMove(const Board& board) const;
    void orderMoves();
    int negamax(Board board, int depth, int color);
};