#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>
#include <iostream>
#include <vector>
#include <chrono>

Ai::Ai(){};
Ai::Ai(int color) : color(color){};
PieceBoard Ai::getMove(Board board)
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    cutoffs = 0;
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
    end = std::chrono::system_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()/1000.0;
    std::cout << searchdepth << " ply deep search returned best move (with score " << bestscore<< ") after " << duration << "s.\nIt considered " << leafnodes << " leaf nodes, and made " << cutoffs << " cutoffs." << std::endl;
    int maxIndex = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
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
        leafnodes += 1;
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
            cutoffs += 1;
            break;
        };
        board.undoMove();
    };
    return bestscore;
};
