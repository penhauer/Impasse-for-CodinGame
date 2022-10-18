#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>
#include <iostream>
#include <vector>

Ai::Ai(){};
Ai::Ai(int color) : color(color){};
PieceBoard Ai::getMove(Board board)
{
    leafnodes = 0;
    PieceBoardVector childnodes = board.possiblepieceboards;
    int bestscore = -100000;
    std::vector<int> scores;
    for (auto childnode : childnodes)
    {
        board.doMove(childnode);
        int score = -alphaBetaNegaMax(board, searchdepth-1, -color, -100000, 100000);
        scores.push_back(score);
        if (score > bestscore)
        {
            bestscore = score;
        }
        board.undoMove();
    };
    int maxIndex = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
    std::cout << "Search depth: " << searchdepth << ", leaf nodes considered: " << leafnodes << ", score: " << bestscore << std::endl;
    return childnodes[maxIndex];
};
PieceBoard Ai::randomMove(const Board &board) const
{
    const PieceBoardVector &moves = board.possiblepieceboards;
    int random = rand() % moves.size();
    return moves[random];
};
void Ai::orderMoves()
{
    

};
int Ai::negamax(Board board, int depth, int color)
{
    if (depth == 0 || board.state != 0)
    {
        leafnodes += 1;
        return board.evaluate(color);
    };
    int bestscore = -10000;
    board.createPossibleBoards();
    PieceBoardVector childnodes = board.possiblepieceboards;
    for (PieceBoard child : childnodes)
    {
        board.doMove(child);
        int score = -negamax(board, depth - 1, -color);
        if (score > bestscore)
        {
            bestscore = score;
        };
        board.undoMove();
    };
    return bestscore;
};
int Ai::alphaBetaNegaMax(Board board, int depth, int color, int alpha, int beta)
{
    PieceBoard bestpieceboard;
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate(color);
    };
    int bestscore = -10000;
    board.createPossibleBoards();
    PieceBoardVector childnodes = board.possiblepieceboards;
    for (PieceBoard child : childnodes)
    {
        board.doMove(child);
        int score = -alphaBetaNegaMax(board, depth - 1, -color, -beta, -alpha);
        if (score > bestscore)
        {
            bestscore = score;
        };
        if (bestscore > alpha)
        {
            alpha = bestscore;
        };
        if (bestscore >= beta)
        {
            break;
        };
        board.undoMove();
    };
    return bestscore;
};
