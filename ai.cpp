#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>
#include <iostream>

Ai::Ai(){};
Ai::Ai(int color) : color(color){};
PieceBoard Ai::getMove(Board board)
{
    //Board new_board = board;
    float bestScore = negamax(board, 3, color);
    //float bestScore = alphaBetaNegaMax(board, 15, color, -100000, 100000);
    // bestPieceBoard = randomMove(board);
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
    PieceBoard bestpieceboard;
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate();
    };
    float bestscore = -1000000.0;
    PieceBoardVector possiblepieceboards = board.possiblepieceboards;
    for (PieceBoard pieceboard : possiblepieceboards)
    {
        board.doMove(pieceboard);
        //float score;
        //PieceBoard pieceboardchild;
        float score = -negamax(board, depth - 1, -color);
        if (score > bestscore)
        {
            bestscore = score;
            bestpieceboard = pieceboard;
        };
        board.undoMove();
    };
    bestPieceBoard = bestpieceboard;
    return bestscore;
};
float Ai::alphaBetaNegaMax(Board &board, int depth, float alpha, float beta, int color)
{
    PieceBoard bestpieceboard;
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate();
    };
    int bestscore = -1000000;
    PieceBoardVector possiblepieceboards = board.possiblepieceboards;
    for (PieceBoard &pieceboard : possiblepieceboards)
    {
        board.doMove(pieceboard);
        float score = -alphaBetaNegaMax(board, depth - 1, -beta, -alpha, -color);
        if (score > bestscore)
        {
            bestscore = score;
            bestpieceboard = pieceboard;
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
    bestPieceBoard = bestpieceboard;
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