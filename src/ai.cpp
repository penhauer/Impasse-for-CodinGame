#include "gamestate.h"

class Ai
{
private:
    int color;
    GameState gamestate;
    MoveMap currentMoves;
public:
    Ai(int color) : color(color) {};
    MoveMap orderMoves() {
        MoveMap orderedMoves;
        return orderMoves;
    };
    MoveMap getMoves() {
        return gamestate.getMoves(color);
    };
    int evaluateBoard() {
        Board board = gamestate.board;
        return color * (board.whiteSingles + board.whiteDoubles - board.blackSingles - board.blackDoubles);
        // TODO add control, number of available moves, etc.
    };
    Board negamax(Board board, int depth, int color) {
        if (depth == 0 || gamestate.state != 0) {
            return evaluateBoard();

        int best_score = -1000000;
        Board best_move_board;
        for (Move move : currentMoves) {
            board.makeMove(move);
            int score = -negamax(board, depth - 1, -color);
            if (score > best_score) {
                best_score = score;
                best_move_board = board;
                };
            };
        return best_move_board;
        };
    };
    void transPositionTable() {
        // TODO
    };
    void moveOrdering() {
        // TODO
    };
    void iterativeDeepening() {
        // TODO
    };
    void alphaBetaPruning() {
        // TODO
    };
    void openingBook() {
        // TODO
    };
    void endgameDatabase() {
        // TODO
    };
};