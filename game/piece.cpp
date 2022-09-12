#include "common.h"

class Piece
{
    private:
        int color; // 1 = white, -1 = black
        int x; //x coordinate
        int y; //y coordinate
        bool isDouble; // true = double, false = single
};
{
    public:
        Piece(int color, int x, int y, bool isDouble)
        {
            this->color = color;
            this->x = x;
            this->y = y;
            this->isDouble = isDouble;
        }
        void changeType(bool isDouble)
        {
            this->isDouble = isDouble;
        }
        void move(int newX, int newY)
        {
            this->x = newX;
            this->y = newY;
        }
        bool getType()
        {
            return isDouble
        }
        int getColor()
        {
            return color
        }
        void getMoves(int board[8][8])
        {
            //return array of possible moves

        }
};