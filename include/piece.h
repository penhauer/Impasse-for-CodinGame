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
    bool getType();
    int getColor();
    // setters
    void setType(bool isDouble);
    void move(int newX, int newY);
};
