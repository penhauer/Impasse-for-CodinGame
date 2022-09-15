#pragma once

struct move {
    int x;
    int y;
    int newX;
    int newY;
    };

class GameState {
private:
    int turn;
    int gameState;
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
    Piece board[8][8];
public:
    void newGame();
    void getMoves(int color);
    void checkDiagonals(int x, int y, bool forward);
};