#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "board.h"

Piece::Piece()
{
    piece = 0;
    color = 0;
    row = -1;
    col = -1;
};
Piece::Piece(int piece, int color, int row, int col) : piece(piece), color(color), row(row), col(col){getDirection();};
void Piece::getDirection() { direction = (piece == 1 && color == 1 || piece == 2 && color == -1) ? 1 : -1; };
void Piece::changeType()
{
    piece = (piece == 2) ? 1 : 2;
    getDirection();
};
bool Piece::operator<(const Piece &other) const
{
    return std::tie(piece, color, row, col) < std::tie(other.piece, other.color, other.row, other.col);
};
bool Piece::operator==(const Piece &other) const
{
    return std::tie(row, col, color, piece) == std::tie(other.row, other.col, other.color, other.piece);
};

Move::Move()
{
    from = Piece();
    to = Piece();
    remove = Piece();
};
Move::Move(Piece from, Piece to = Piece(0, 0, -1, -1), Piece remove = Piece(0, 0, -1, -1)) : from(from), to(to), remove(remove){};
bool Move::operator<(const Move &other) const
{
    return std::tie(from, to, remove) < std::tie(other.from, other.to, other.remove);
};
bool Move::operator==(const Move &other) const
{
    return from == other.from && to == other.to && remove == other.remove;
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
    initBoard();
    getPieceCount();
    boolPieceToCrown = false;
    piecetocrown.clear();
    createMoveSet();
};
void Board::saveBoard() const
{
    std::ofstream newfile;
    newfile.open("boardstate.txt", std::ios::out);
    if (newfile.is_open())
    {
        newfile << turn << std::endl;
        newfile << state << std::endl;
        newfile << boolPieceToCrown << std::endl;
        newfile << piecetocrown.size() << std::endl;
        //TODO add piecetocrown and pieceset
    };
    newfile.close();
};
void Board::loadBoard()
{
    std::ifstream newfile;
    newfile.open("boardstate.txt", std::ios::in);
    if (newfile.is_open())
    {
        newfile >> turn;
        newfile >> state;
        newfile >> boolPieceToCrown;
        int size;
        newfile >> size;
        //TODO add piecetocrown and pieceset
    };
    newfile.close();
};
//If saved board exists, delete it
void Board::deleteBoard() const
{
    std::ifstream newfile;
    newfile.open("boardstate.txt", std::ios::in);
    if (newfile.is_open())
    {
        newfile.close();
        std::remove("boardstate.txt");
    };
};
//Print board to console
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
//Print current valid moves to console
void Board::printMoves() const
{
    for (const auto &move : moveset)
    {
        std::cout << "From " << reverseParseMove(move.from.row, move.from.col);
        if (move.to.col != -1 && move.to.row != -1)
        {
            std::cout << " to " << reverseParseMove(move.to.row, move.to.col);
        };
        if (move.remove.col != -1 && move.remove.row != -1)
        {
            std::cout << " removing " << reverseParseMove(move.remove.row, move.remove.col);
        };
        std::cout << std::endl;
    };
};
//Create current valid moves
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
//Remove no longer valid moves, add new possible moves WIP
void Board::updateMoveSet(const Move &move)
{
    const Piece &from = move.from;
    const Piece &to = move.to;
    const Piece &remove = move.remove;
    // TODO
};
//Evaluate heuristic value of current board
float Board::evaluate() const
{
    float score = turn * (1.5 * (piececount.blackSingles + piececount.blackDoubles) - piececount.whiteSingles - piececount.whiteDoubles);
    return score;
    // TODO add average number of available moves per piece
};
//Update board state with selected move
void Board::doMove(const Move &move)
{
    // Impasse
    if (move.from == move.remove)
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
    else if (move.to.row == 0 || move.to.row == 7)
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
    Piece tempfrom = piecemap.at(move.from.row).at(move.from.col);
    tempfrom.row = move.to.row;
    tempfrom.col = move.to.col;
    if (move.to.piece > 0)
    {
        piecemap.at(move.to.row).at(move.to.col).row = piecemap.at(move.from.row).at(move.from.col).row;
        piecemap.at(move.to.row).at(move.to.col).col = piecemap.at(move.from.row).at(move.from.col).col;
        piecemap.at(move.from.row).at(move.from.col) = piecemap.at(move.to.row).at(move.to.col);
    }
    else
    {
        piecemap.at(move.from.row).erase(move.from.col);
    }
    piecemap[move.to.row][move.to.col] = tempfrom;
    turn = turn * -1;
    createMoveSet();
};
//Crown piece
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
    piece.getDirection();
};
//Bear-off piece
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
    piece.getDirection();
};
//Remove / bear-off piece based on type after impasse
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
    // Chickendinner
    if (piececount.whiteSingles + piececount.whiteDoubles == 0)
    {
        state = 1;
    }
    else if (piececount.blackSingles + piececount.blackDoubles == 0)
    {
        state = -1;
    };
};
//Create set of current color singles
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
                singles.insert(p);
            };
        };
    };
    return singles;
};
bool Board::ifTransposable(const Piece &piece, const int &i, const int &row, const int &col) const
{
    return ((i == 1 || i == -1) && piecemap.count(row) > 0 && piecemap.at(row).count(col) > 0 && piecemap.at(row).at(col).color == turn && piece.piece + piecemap.at(row).at(col).piece == 3);
};
bool Board::ifEmptySquare(const Piece &piece, const int &row, const int &col) const
{
    return (piecemap.count(row) == 0 || (piecemap.count(row) > 0 && piecemap.at(row).count(col) == 0));
};
void Board::addPieceDiagonals(const Piece &piece)
{
    // to cover for left and right search
    for (int sign = 0; sign < 2; sign++)
    {
        int i = piece.direction;
        while (i + piece.row < 8 && piece.row + i >= 0)
        {
            int row = piece.row + i;
            int col = piece.col + i*pow(-1, sign);
            if (col < 0 || col > 7)
            {
                break;
            };
            //slide or transpose
            bool transposable = ifTransposable(piece, i, row, col);
            bool emptysquare = ifEmptySquare(piece, row, col);
            if (emptysquare || transposable)
            {
                Piece square = transposable ? piecemap.at(row).at(col) : Piece(0, 0, row, col);
                // crown
                if ((row == 0 || row == 7) && piece.piece == 1)
                {
                    PieceSet singles = checkSingles(piece);
                    // if other single available
                    if (singles.size() > 0)
                    {
                        for (auto removepiece : singles)
                        {
                            moveset.insert(Move(piece, square, removepiece));
                        }
                    }
                    // if no other singles available, just add as normal slide (and add to crownstack)
                    else
                    {
                        moveset.insert(Move(piece, square));
                    }
                }
                // bear-off / simple slide
                else
                {
                    moveset.insert(Move(piece, square));
                };
                //if it was a transpose, break
                if (transposable)
                {
                    break;
                };
            }
            //if the square was not empty or transposable, break
            else
            {
                break;
            };
            //otherwise increment i and continue loop
            i = i + piece.direction;
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
void Board::initBoard()
{
    deleteBoard();
    piecemap.clear();
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
                break;
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
