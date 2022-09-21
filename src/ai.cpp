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
        currentMoves = gamestate.getMoves(color);
        return currentMoves;
    };
    void evaluate(board) {
        // evaluate board
    };
};