#include "board.h"

class Ai
{
public:
    Ai();
    Ai(int color) {this->color = color;};
    Move getMove(const Board &board) const
    {
        int score = negamax(board, 3, color);
        return bestMove;
    };
private:
    int color;
    Move bestMove;
    int evaluateBoard(Board board) const
    {
        const PieceCount &piececount = board.piececount;
        return color * (piececount.whiteSingles + piececount.whiteDoubles - piececount.blackSingles - piececount.blackDoubles);
        // TODO add control, number of available moves, etc.
    };
    void orderMoves()
    {
        MoveSet orderedMoves;
    };
    int negamax(Board board, int depth, int color) const
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
    void transPositionTable(){
        // TODO
    };
    void moveOrdering(){
        // TODO
    };
    void iterativeDeepening(){
        // TODO
    };
    void alphaBetaPruning(){
        // TODO
    };
    void openingBook(){
        // TODO
    };
    void endgameDatabase(){
        // TODO
    };
};