#pragma once
#include <fstream>
#include <string>
#include <map>

typedef BoardArray;
struct PieceCount {
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
};

class Board
{
public:
    BoardArray boardarray;
    PieceCount piececount;
    Board();
    void save_board();
    void delete_board();
private:
    BoardArray get_board(bool paused);
    PieceCount get_pieces();
};