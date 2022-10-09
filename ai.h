#pragma once

class Ai
{
private:
    int color;
    PieceBoard bestPieceBoard;

public:
    Ai();
    Ai(int color);
    PieceBoard getMove(Board board);
private:
    PieceBoard randomMove(const Board &board) const;
    void orderMoves();
    float negamax(Board &board, int depth, int color);
    float alphaBetaNegaMax(Board &board, int depth, float alpha, float beta, int color);
};