class Piece
{
private:
    int color;
    int x;
    int y;
    bool isDouble;
public:
    // constructor
    Piece(int color, int x, int y, bool isDouble);

    // getters
    bool getType() { return isDouble; }
    int getColor() { return color; }
    // setters
    void setDouble(bool isDouble) { this->isDouble = isDouble; };
    void move(int newX, int newY) { x = newX; y = newY; }
};
