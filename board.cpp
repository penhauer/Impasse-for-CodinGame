#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "board.h"

Piece::Piece(){};
Piece::Piece(int piece, int color, int row, int col) : piece(piece), color(color), row(row), col(col){};
void Piece::getDirection() { direction = (piece == 1 && color == 1 || piece == 2 && color == -1) ? 1 : -1; };
void Piece::changeType()
{
    piece = (piece == 2) ? 1 : 2;
    getDirection();
};

Move::Move(){};
Move::Move(Piece from, Piece to, Piece remove = Piece(0, 0, -1, -1)) : from(from), to(to), remove(remove){};
bool Move::validMove() const
{
    return from.col >= -1 && to.col >= -1 && remove.col >= -1;
};

namespace std
{
    template <>
    struct hash<Move>
    {
        std::size_t operator()(const Move &k) const
        {
            using std::hash;
            using std::size_t;
            using std::string;
            return ((hash<int>()(k.from.piece) ^ (hash<int>()(k.from.color) << 1)) >> 1) ^ ((hash<int>()(k.to.piece) ^ (hash<int>()(k.to.color) << 1)) >> 1) ^ ((hash<int>()(k.remove.piece) ^ (hash<int>()(k.remove.color) << 1)) >> 1);
        }
    };
};

Board::Board(){};
Board::Board(bool paused)
{
    resetBoard(paused);
};
void Board::resetBoard(bool paused)
{
    turn = 1;
    state = 0;
    initBoard(paused);
    getPieceCount();
    createMoveSet();
    boolPieceToCrown = false;
    piecetocrown.clear();
};
void Board::restoreLast(const Board &b){
    // TODO
};
void Board::saveBoard() const
{
    std::ofstream newfile;
    newfile.open("boardstate.txt", std::ios::out);
    if (newfile.is_open())
    {
        for (int i = 0; i < 64; i++)
        {
            int col = i % 8;
            int row = i / 8;
            // newfile << piecemap.at(col).at(row) << std::endl; TODO
        }
        newfile.close();
    };
};
void Board::deleteBoard() const
{
    std::remove("boardstate.txt");
};
void Board::printBoard() const
{
    for (int row = 7; row >= 0; row--)
    {
        std::cout << row + 1 << " ";
        for (int col = 0; col < 8; col++)
        {
            if (piecemap.count(row) > 0 && piecemap.at(row).count(col) > 0)
            {
                const Piece &piece = piecemap.at(row).at(col);
                switch (piece.piece)
                {
                case 1:
                    switch (piece.color)
                    {
                    case 1:
                        std::cout << "○ ";
                        break;
                    case -1:
                        std::cout << "● ";
                        break;
                    }
                    break;
                case 2:
                    switch (piece.color)
                    {
                    case 1:
                        std::cout << "◇ ";
                        break;
                    case -1:
                        std::cout << "◆ ";
                        break;
                    }
                    break;
                }
            }
            else
            {
                std::cout << "  ";
            }
        };
        std::cout << std::endl;
    };
    std::cout << "  A B C D E F G H" << std::endl;
};
void Board::printMoves() const
{
    for (const auto &move : moveset)
    {
        std::cout << move.from.col << move.from.row << " " << move.to.col << move.to.row << " " << move.remove.col << move.remove.row << std::endl;
    }
};
void Board::createMoveSet()
{
    moveset.clear();
    // return array of possible moves
    for (auto x : piecemap)
    {
        const int &row = x.first;
        for (auto y : x.second)
        {
            const int &col = y.first;
            const Piece &piece = y.second;
            if (piece.color == turn)
            {
                addPieceDiagonals(piece);
            }
        };
    };
    // if no move, impasse
    if (moveset.size() == 0)
    {
        addImpassable();
    };
};
void Board::updateMoveSet(const Move &move)
{
    const Piece &from = move.from;
    const Piece &to = move.to;
    const Piece &remove = move.remove;
    // TODO remove moves of the previous sols, add moves of the new sols
}
void Board::doMove(const Move &move)
{
    // Impasse
    if (move.from.col == move.remove.col && move.from.row == move.remove.row)
    {
        if (move.remove.piece == 2)
        {
            remove(move.from);
            crownIf(Piece(1, move.from.color, move.from.col, move.from.row));
        }
        else
        {
            remove(move.from);
        };
    }
    // bear off and crown
    else if (move.to.row == 0 || move.to.row == 8)
    {
        // bear-off
        if (move.from.piece == 2)
        {
            bearOff(move.from);
            crownIf(move.from);
        }
        // crown if single was available
        else if (move.remove.piece == 1)
        {
            crown(move.from);
            remove(move.remove);
        }
        // otherwise put crownable to stack
        else
        {
            piecetocrown[turn] = move.to;
            boolPieceToCrown = true;
        }
    };
    Piece from = piecemap.at(move.from.row).at(move.from.col);
    piecemap[move.from.row][move.from.col] = piecemap.at(move.to.row).at(move.to.col);
    piecemap[move.to.row][move.to.col] = from;
    // boardhistory.push_back(move); TODO
    turn = turn * -1;
    createMoveSet();
};
void Board::undoMove(){
    // Move move = boardhistory.back();
    // boardhistory.pop_back();
};
void Board::crown(const Piece &p)
{
    Piece &piece = piecemap.at(p.row).at(p.col);
    switch (piece.color)
    {
    case 1:
        piececount.whiteDoubles++;
        piececount.whiteSingles--;
        break;
    case -1:
        piececount.blackDoubles++;
        piececount.blackSingles--;
        break;
    };
    piece.piece = 2;
};
void Board::bearOff(const Piece &p)
{
    Piece &piece = piecemap.at(p.row).at(p.col);
    switch (p.color)
    {
    case 1:
        piececount.whiteDoubles--;
        piececount.whiteSingles++;
        break;
    case -1:
        piececount.blackDoubles--;
        piececount.blackSingles++;
        break;
    };
    piece.piece = 1;
};
// For impasse / bear-off
void Board::remove(const Piece &p)
{
    switch (p.piece)
    {
    case 1:
        switch (p.color)
        {
        case 1:
            piececount.whiteSingles--;
            break;
        case -1:
            piececount.blackSingles--;
            break;
        };
        piecemap.at(p.row).erase(p.col);
        break;
    // bear-off
    case 2:
        //bearoff
        bearOff(p);
        break;
    };
    // chickendinner
    if (piececount.whiteSingles + piececount.whiteDoubles == 0)
    {
        state = 1;
    }
    else if (piececount.blackSingles + piececount.blackDoubles == 0)
    {
        state = -1;
    };
};
PieceSet Board::checkSingles(const Piece &piece) const
{
    PieceSet singles;
    for (auto x : piecemap)
    {
        const int &row = x.first;
        for (auto y : x.second)
        {
            const int &col = y.first;
            Piece p = y.second;
            if (p.piece == 1 && p.color == turn && (p.col != piece.col && p.row != piece.row))
            {
                singles.insert(piece);
            };
        };
    };
    return singles;
};
void Board::addPieceDiagonals(const Piece &piece)
{
    int i = piece.direction;
    // to cover for left and right search
    for (int sign = 0; sign < 2; sign++)
    {
        while (i + piece.row < 8 && piece.row - i >= 0)
        {
            int row = piece.row + i;
            int col = piece.col + i*(-1)^sign;
            // transpose
            if ((i == 1 || i == -1) && piece.piece == 2 && piecemap.count(row) > 0 && piecemap.at(row).count(col) > 0 && piecemap.at(row).at(col).piece == 1)
            {
                const Piece &newpiece = piecemap.at(row).at(col);
                // crown
                if (row == 0 || row == 8)
                {
                    // at least one single is guaranteed to be available, since the double transposes with one
                    PieceSet singles = checkSingles(piece);
                    for (auto removepiece : singles)
                    {
                        moveset.insert(Move(piece, newpiece, removepiece));
                    }
                }
                // bear-off
                else
                {
                    moveset.insert(Move(piece, newpiece));
                }
                break;
            };
            // slide
            if (piecemap.count(row) == 0 || (piecemap.count(row) > 0 && piecemap.at(row).count(col) == 0))
            {
                if (row == 0 || row == 8)
                {
                    // crown
                    if (piece.piece == 1)
                    {
                        PieceSet singles = checkSingles(piece);
                        // if other single available
                        if (singles.size() > 0)
                        {
                            for (auto removepiece : singles)
                            {
                                moveset.insert(Move(piece, Piece(1, piece.color, col, row), removepiece));
                            }
                        }
                        else
                        {
                            // if no other singles available
                            moveset.insert(Move(piece, Piece(1, piece.color, col, row)));
                        }
                    }
                    // bear-off
                    else
                    {
                        moveset.insert(Move(piece, Piece(0, 0, row, col)));
                    }
                }
                else
                {
                    moveset.insert(Move(piece, Piece(0, 0, row, col)));
                };
                i = i + piece.direction;
            }
            else
            {
                break;
            };
        };
    };
};
void Board::crownIf(const Piece &p)
{
    if (boolPieceToCrown)
    {
        const Piece &piece = piecetocrown[turn];
        crown(piece);
        piecetocrown.erase(turn);
        boolPieceToCrown = false;
        remove(p);
    }
};
void Board::addImpassable()
{
    for (auto x : piecemap)
    {
        int row = x.first;
        for (auto y : x.second)
        {
            int col = y.first;
            Piece piece = y.second;
            if (piece.piece * turn > 0)
            {
                moveset.insert(Move(piece, Piece(0, 0, -1, -1), piece));
            };
        }
    }
};
void Board::initBoard(bool paused)
{
    if (paused)
    {
        std::ifstream newfile;
        std::string line;
        newfile.open("boardstate.txt", std::ios::in);
        if (newfile.is_open())
        {
            std::string tp;
            for (int i = 0; i < 64; i++)
            {
                int col = i % 8;
                int row = i / 8;
                getline(newfile, tp);
                int piece = std::stoi(tp);
                getline(newfile, tp);
                int color = std::stoi(tp);
                if (piece != 0)
                {
                    piecemap[row][col] = Piece(piece, color, row, col);
                }
            }
            newfile.close();
        };
    }
    else
    {
        // deleteBoard(); TODO
        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                int pos = row * 8 + col;
                if (pos == 0 || pos == 4 || pos == 11 || pos == 15)
                {
                    piecemap[row][col] = Piece(1, 1, row, col);
                }
                else if (pos == 50 || pos == 54 || pos == 57 || pos == 61)
                {
                    piecemap[row][col] = Piece(2, 1, row, col);
                }
                else if (pos == 48 || pos == 52 || pos == 59 || pos == 63)
                {
                    piecemap[row][col] = Piece(1, -1, row, col);
                }
                else if (pos == 2 || pos == 6 || pos == 9 || pos == 13)
                {
                    piecemap[row][col] = Piece(2, -1, row, col);
                }
            }
        };
    };
};
void Board::getPieceCount()
{
    for (auto x : piecemap)
    {
        for (auto y : x.second)
        {
            const Piece &piece = y.second;
            switch (piece.piece)
            {
            case 1:
                switch (piece.color)
                {
                case 1:
                    piececount.whiteSingles++;
                    break;
                case -1:
                    piececount.blackSingles++;
                    break;
                };
            case 2:
                switch (piece.color)
                {
                case 1:
                    piececount.whiteDoubles++;
                    break;
                case -1:
                    piececount.blackDoubles++;
                    break;
                };
                break;
            };
        };
    };
};
void Board::getIfToCrown()
{
    // TODO for when setting up the board again, or just save the whole boardstate
}