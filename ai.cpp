#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>

Ai::Ai(){};
Ai::Ai(int color) : color(color){};
PieceBoard Ai::getMove(Board board)
{
    PieceBoard bestPieceBoard;
    //int bestScore = negamax(&board, 3, color);
    bestPieceBoard = randomMove(board);
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
float Ai::negamax(Board board, int depth, int color)
{
    if (depth == 0 || board.state != 0)
    {
        return board.evaluate();
    };
    int bestscore = -1000000;
    for (const PieceBoard &pieceboard : board.possiblepieceboards)
    {
        board.doMove(pieceboard);
        int score = -negamax(board, depth - 1, -color);
        if (score > bestscore)
        {
            bestscore = score;
            bestPieceBoard = pieceboard;
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
void Ai::alphaBetaPruning() {
    // TODO
};
void Ai::openingBook() {
    // TODO
};
void Ai::endgameDatabase() {
    // TODO
};*/