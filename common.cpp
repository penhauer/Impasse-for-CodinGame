#include "common.h"

Piece::Piece() {};
Piece::Piece(int piece, int color, int pos) : piece(piece), color(color) { transitions = 0; getDirection(); getDistance(pos); };
void Piece::getDirection() { direction = (piece == 1 && color == 1 || piece == 2 && color == -1) ? 1 : -1; };
void Piece::getDistance(const int &pos) { const int row = pos / 8; distance = (direction == 1) ? 7 - row : row; };
Move::Move() {};
Move::Move(int from = -1, int to = -1, int remove = -1) : from(from), to(to), remove(remove){};
bool Move::operator<(const Move &other) const { return from < other.from && to < other.to && remove < other.remove; };

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

// Print current valid moves to console
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
