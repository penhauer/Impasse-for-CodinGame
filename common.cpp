#include "common.h"

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
