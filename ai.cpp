#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>

Ai::Ai(){};
Ai::Ai(int color) : color(color){};
Move Ai::getMove(Board board)
{
    Move bestMove = Move();
    int bestScore = negamax(&board, 3, color);
    return bestMove;
};
Move Ai::randomMove(const Board &board) const
{
    const MoveSet &moves = board.moveset;
    int r = rand() % moves.size();
    auto it = std::begin(moves);
    std::advance(it, r);
    return *it;
};
void Ai::orderMoves()
{
    MoveSet orderedMoves;
};
int Ai::negamax(Board board, int depth, int color)
{
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate();
    };
    int bestscore = -1000000;
    for (const Move &move : board.moveset)
    {
        board.doMove(move);
        int score = -negamax(board, depth - 1, -color);
        if (score > bestscore)
        {
            bestscore = score;
            bestMove = move;
        };
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
void Ai::alphaBetaPruning() {
    // TODO
};
void Ai::openingBook() {
    // TODO
};
void Ai::endgameDatabase() {
    // TODO
};*/