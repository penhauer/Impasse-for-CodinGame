#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>

Ai::Ai() {};
Ai::Ai(int color) : color(color) {};
Move Ai::getMove(const Board& board)
{
    //int score = negamax(board, 3, color);
    //return bestMove;
    return randomMove(board);
};
Move Ai::randomMove(const Board& board) const
{
    const MoveSet &moves = board.moveset;
    int r = rand() % moves.size();
    auto it = std::begin(moves);
    // 'advance' the iterator n times
    std::advance(it,r);
    return *it;

};
int Ai::evaluateBoard(Board board) const
{
    const PieceCount& piececount = board.piececount;
    return color * (piececount.whiteSingles + piececount.whiteDoubles - piececount.blackSingles - piececount.blackDoubles);
    // TODO add control, number of available moves, etc.
};
void Ai::orderMoves()
{
    MoveSet orderedMoves;
};
int Ai::negamax(Board board, int depth, int color)
{
    /*if (depth == 0 || board.state != 0)
    {
        return evaluateBoard(board);
    };
    int best_score = -1000000;
    Board bestMoveBoard;
    for (auto const &[from, toset] : currentMoves)
    {
        for (auto const &to : toset)
        {
            Move move = Move(from, to);
            board.doMove(move);
            int score = -negamax(board, depth - 1, -color);
            if (score > best_score)
            {
                best_score = score;
                bestMove = move;
            }
            board.undoMove();
        }
    };*/
    return 0;
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