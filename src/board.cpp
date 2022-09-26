#include <fstream>
#include <string>
#include <vector>

typedef int BoardArray[64];

struct PieceCount
{
    int whiteSingles; // number of white singles
    int whiteDoubles; // number of white doubles
    int blackSingles; // number of black singles
    int blackDoubles; // number of black doubles
};

class Board
{
public:
    BoardArray boardarray;
    PieceCount piececount;
    Board()
    {
        boardarray = get_board(false);
        piececount = PieceCount();
    };
    void save_board()
    {
        std::ofstream newfile;
        newfile.open("boardstate.txt",std::ios::out);
        if (newfile.is_open())
        {
            for (int i = 0; i < 64; i++)
            {
                newfile << boardarray.at(i) << std::endl;
            }
        newfile.close();
        };
    };
    void delete_board()
    {
        std::remove("boardstate.txt");
    };
private:
    BoardArray get_board(bool paused)
    {
        BoardArray boardarray;
        if (paused)
        {
            std::ifstream newfile;
            std::string line;
            newfile.open("boardstate.txt",std::ios::in);
            if (newfile.is_open())
            {
                std::string tp;
                for (int i = 0; i < 64; i++)
                {
                    getline(newfile, tp);
                    boardarray[i] = std::stoi(tp);
                }
            newfile.close();
            };
            return boardarray;
        };
        //else create new board
        for (int pos = 0; pos < 8; pos++)
        {
            if (pos == 0 || pos == 4 || pos == 11 || pos == 15)
            {
                boardarray[pos] = 1;
            }
            else if (pos == 50 || pos == 54 || pos == 57 || pos == 61)
            {
                boardarray[pos] = 2;
            }
            else if (pos == 48 || pos == 52 || pos == 59 || pos == 63)
            {
                boardarray[pos] = -1;
            }
            else if (pos == 2 || pos == 6 || pos == 9 || pos == 13)
            {
                boardarray[pos] = -2;
            }
            else
            {
                boardarray[pos] = 0;
            };
        };
        return boardarray;
    };
    PieceCount get_pieces()
    {
        PieceCount piececount;
        for (int pos; pos < 64; pos++)
        {
            const int &piece = boardarray.at(pos);
            if (piece == 1)
            {
                piececount.whiteSingles++;
            }
            else if (piece == 2)
            {
                piececount.whiteDoubles++;
            }
            else if (piece == -1)
            {
                piececount.blackSingles++;
            }
            else if (piece == -2)
            {
                piececount.blackDoubles++;
            }
        };
        return piececount;
    };
};