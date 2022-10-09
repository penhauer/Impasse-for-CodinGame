#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>
#include <iostream>

Ai::Ai(){};
Ai::Ai(int color) : color(color){};
PieceBoard Ai::getMove(Board board)
{
    PieceBoard bestPieceBoard;
    Board new_board = board;
    int bestScore = negamax(new_board, 1, color);
    //bestPieceBoard = randomMove(board);
    return bestPieceBoard;
};
PieceBoard Ai::randomMove(const Board &board) const
{
    const PieceBoardVector &moves = board.possiblepieceboards;
    int random = rand() % moves.size();
    return moves[random];
};
void Ai::orderMoves()
{
    PieceBoardVector orderedMoves;
};
float Ai::negamax(Board &board, int depth, int color)
{
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate();
    };
    int bestscore = -1000000;
    for (PieceBoard &pieceboard : board.possiblepieceboards)
    {
        board.doMove(pieceboard);
        float score = -negamax(board, depth - 1, -color);
        if (score > bestscore)
        {
            bestscore = score;
            bestPieceBoard = pieceboard;
        };
        board.undoMove();
    };
    return bestscore;
};
float Ai::alphaBetaNegaMax(Board &board, int depth, float alpha, float beta, int color) {
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate();
    };
    int bestscore = -1000000;
    for (PieceBoard &pieceboard : board.possiblepieceboards)
    {
        board.doMove(pieceboard);
        float score = -alphaBetaNegaMax(board, depth - 1, -beta, -alpha, -color);
        if (score > bestscore)
        {
            bestscore = score;
            bestPieceBoard = pieceboard;
        };
        if (bestscore > alpha)
        {
            alpha = bestscore;
        };
        if (alpha >= beta)
        {
            break;
        };
        board.undoMove();
    };
    return bestscore;
};
/*void Ai::transPositionTable() {
    // TODO
};
void Ai::moveOrdering() {
    // TODO
};
void Ai::iterativeDeepening() {
    // TODO
};
void Ai::openingBook() {
    // TODO
};
void Ai::endgameDatabase() {
    // TODO
};*/