#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>
#include <iostream>

Ai::Ai(){};
Ai::Ai(int color) : color(color){};
PieceBoard Ai::getMove(Board board)
{
    leafnodes = 0;
    //Board new_board = board;
    //bool maxplayer = 1 ? color == 1 : 0;
    //int bestScore = miniMax(board, 3, maxplayer);
    int score = negamax(board, searchdepth, color);
    std::cout << "Search depth: " << searchdepth << ", leaf nodes considered: " << leafnodes << ", score: " << score << std::endl;
    //int bestScore = alphaBetaNegaMax(board, 15, color, -100000, 100000);
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
int Ai::negamax(Board &board, int depth, int color)
{
    if (depth == 0 || board.state != 0)
    {
        leafnodes += 1;
        return board.evaluate(color);
    };
    int bestscore = -1000000;
    board.createPossibleBoards();
    PieceBoardVector childnodes = board.possiblepieceboards;
    for (PieceBoard child : childnodes)
    {
        if (child.lastmove.from == 52 && child.lastmove.to == 16)
        {
            int a = 1;
        }
        board.doMove(child);
        int score = -negamax(board, depth - 1, -color);
        if (score > bestscore)
        {
            bestscore = score;
            if (depth == searchdepth)
            {
                bestPieceBoard = child;
            };
        };
        board.undoMove();
    };
    return bestscore;
};
int Ai::alphaBetaNegaMax(Board &board, int depth, int alpha, int beta, int color)
{
    PieceBoard bestpieceboard;
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate(color);
    };
    int bestscore = -1000000;
    PieceBoardVector possiblepieceboards = board.possiblepieceboards;
    for (PieceBoard &pieceboard : possiblepieceboards)
    {
        board.doMove(pieceboard);
        int score = alphaBetaNegaMax(board, depth - 1, -beta, -alpha, -color);
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