#pragma once

class Piece
{
private:
    int color;
    bool isDouble;
public:
    // constructor
    Piece(int color, bool isDouble);
    // getters
    bool getType();
    int getColor();
    // setters
    void setType(bool isDouble);
};
