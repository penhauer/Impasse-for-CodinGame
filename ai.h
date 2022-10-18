#pragma once

class Ai
{
private:
    int color;
    PieceBoard bestPieceBoard;
    int leafnodes = 0;
    int searchdepth = 5;

public:
    Ai();
    Ai(int color);
    PieceBoard getMove(Board board);
private:
    PieceBoard randomMove(const Board &board) const;
    void orderMoves();
    int miniMax(Board &board, int depth, bool maxplayer);
    int negamax(Board board, int depth, int color);
    int alphaBetaNegaMax(Board board, int depth, int color, int alpha, int beta);
};