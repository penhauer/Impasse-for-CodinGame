#pragma once

class Board {
public:
    Board();
    ~Board();
    void draw();
    void update();
    void handleInput();
    void checkCollision();
    void reset();
    bool isGameOver();
    bool isGameWon();
    