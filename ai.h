class Ai
{
public:
    Ai();
    Ai(int color);
    Move getMove(const Board &board) const;
private:
    int evaluateBoard() const;
    void orderMoves();
};