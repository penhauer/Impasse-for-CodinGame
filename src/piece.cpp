class Piece
{
private:
    int color; // 1 = white, -1 = black
    int x;
    int y;
    bool isDouble; // true = double, false = single
public:
    // constructor
    Piece(int color, int x, int y, bool isDouble)
    {
        this->color = color;
        this->x = x;
        this->y = y;
        this->isDouble = isDouble;
    };
};