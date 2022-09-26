#pragma once
#include <fstream>
#include <string>
#include <map>

typedef Board;
struct PieceCount {
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
};

Board get_board(bool paused);
PieceCount get_pieces();