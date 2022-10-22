#include "common.h"

Piece::Piece(){};
Piece::Piece(int piece, int color, int pos) : piece(piece), color(color)
{
    transitions = 0;
    getDirection();
    getDistance(pos);
};
// Update the direction of the piece
void Piece::getDirection() { direction = (piece == 1 && color == 1 || piece == 2 && color == -1) ? 1 : -1; };
// Update the distance of the piece from its last row
void Piece::getDistance(const int &pos)
{
    const int row = pos / 8;
    distance = (direction == 1) ? 7 - row : row;
};
Move::Move()
{
    from = -1;
    to = -1;
    remove = -1;
};
Move::Move(int from, int to, int remove) : from(from), to(to), remove(remove){};
bool Move::operator<(const Move &other) const { return from < other.from && to < other.to && remove < other.remove; };

// Convert chess notation to board position
int parseMove(const std::string &notation)
{
    if (notation.size() != 2)
    {
        return -1;
    };
    char letter = tolower(notation[0]);
    int row = notation[1] - '1';
    int col = (int)letter - 97;
    row = (int)row;
    int pos = col + 8 * row;
    if (pos < 0 || pos > 63)
    {
        return -1;
    };
    return pos;
};
// Convert board position to chess notation
std::string reverseParseMove(int pos)
{
    int col = pos % 8;
    int row = pos / 8;
    char letter = toupper((char)(col + 97));
    char number = (char)(row + 49);
    std::string notation = "";
    notation += letter;
    notation += number;
    return notation;
};
// Print a single move
void printMove(const Move &move)
{
    std::cout << "From " << reverseParseMove(move.from);
    if (move.to != -1)
    {
        std::cout << " to " << reverseParseMove(move.to);
    };
    if (move.remove != -1)
    {
        std::cout << " removing " << reverseParseMove(move.remove);
    };
    std::cout << std::endl;
};
