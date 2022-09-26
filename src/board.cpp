#include <fstream>
#include <string>
#include <map>

typedef std::map<int, int> BoardMap;

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
    BoardMap boardmap;
    PieceCount piececount;
    Board()
    {
        boardmap = get_board(false);
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
                newfile << boardmap.at(i) << std::endl;
            }
        newfile.close();
        };
    };
    void delete_board()
    {
        std::remove("boardstate.txt");
    };
private:
    BoardMap get_board(bool paused)
    {
        BoardMap boardmap;
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
                    boardmap[i] = std::stoi(tp);
                }
            newfile.close();
            };
            return boardmap;
        };
        //else create new board
        for (int pos = 0; pos < 8; pos++)
        {
            if (pos == 0 || pos == 4 || pos == 11 || pos == 15)
            {
                boardmap[pos] = 1;
            }
            else if (pos == 50 || pos == 54 || pos == 57 || pos == 61)
            {
                boardmap[pos] = 2;
            }
            else if (pos == 48 || pos == 52 || pos == 59 || pos == 63)
            {
                boardmap[pos] = -1;
            }
            else if (pos == 2 || pos == 6 || pos == 9 || pos == 13)
            {
                boardmap[pos] = -2;
            }
            else
            {
                boardmap[pos] = 0;
            };
        };
        return boardmap;
    };
    PieceCount get_pieces()
    {
        PieceCount piececount;
        for (int pos; pos < 64; pos++)
        {
            const int &piece = boardmap.at(pos);
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