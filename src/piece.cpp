#include <iostream>

struct Piece
{
    int color;     // 1 = white, -1 = black
    bool isDouble; // true = double, false = single
    // constructor
    Piece(int color, bool isDouble)
    {
        this->color = color;
        this->isDouble = isDouble;
    };
};