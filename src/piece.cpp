class Piece
{
private:
    int color; // 1 = white, -1 = black
    bool isDouble; // true = double, false = single
public:
    // constructor
    Piece(int color, bool isDouble)
    {
        this->color = color;
        this->isDouble = isDouble;
    };
    // getters
    bool getType() { return isDouble; }
    int getColor() { return color; }
    // setters
    void setType(bool isDouble) { this->isDouble = isDouble; };
};