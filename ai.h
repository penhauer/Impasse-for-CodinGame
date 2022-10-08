#pragma once

class Ai
{
private:
    int color;
    Move bestBoard;

public:
    Ai();
    Ai(int color);
    PieceBoard getMove(Board board);
private:
    PieceBoard randomMove(const Board &board) const;
    void orderMoves();
    float negamax(Board board, int depth, int color);
};