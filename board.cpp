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
    pieceboard.piecetocrown.clear();
    createPossibleBoards();
};
void Board::saveBoard() const
{
    std::ofstream newfile;
    newfile.open("boardstate.txt", std::ios::out);
    if (newfile.is_open())
    {
        newfile << turn << std::endl;
        newfile << state << std::endl;
        newfile << pieceboard.piecetocrown.size() << std::endl;
        //TODO add piecetocrown and pieceset
    };
    newfile.close();
};
void Board::loadBoard(int turn, PieceToCrown piecetocrown, PieceSet pieceset)
{
    state = 0;
    this->turn = turn;
    this->pieceboard.piecetocrown = piecetocrown;
    for (Piece piece : pieceset)
    {
        pieceboard.piecemap[piece.row][piece.col] = piece;
    };
    getPieceCount();
    createPossibleBoards();
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
        std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
        std::cout << row + 1 << " ";
        for (int col = 0; col < 8; col++)
        {
            std::cout << "| ";
            if (pieceboard.piecemap.count(row) > 0 && pieceboard.piecemap.at(row).count(col) > 0)
            {
                const Piece &piece = pieceboard.piecemap.at(row).at(col);
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
                };
            }
            else
            {
                std::cout << "  ";
            };
        };
        std::cout << "|" << std::endl;
    };
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    std::cout << "    A   B   C   D   E   F   G   H " << std::endl;
};
//Print current valid moves to console
void Board::printMove(const Move &move) const
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
//Create current valid moves
void Board::createPossibleBoards()
{
    possibleboards.clear();
    // return array of possible moves
    for (auto x : pieceboard.piecemap)
    {
        const int &row = x.first;
        for (auto y : x.second)
        {
            const int &col = y.first;
            const Piece &piece = y.second;
            if (piece.color == turn)
            {
                addPieceMoves(piece);
            }
        };
    };
    // if no move, impasse
    if (possibleboards.size() == 0)
    {
        addImpassable();
    };
};
//Evaluate heuristic value of current board
float Board::evaluate() const
{
    float score = turn * (1.5 * (pieceboard.piececount.blackSingles + pieceboard.piececount.blackDoubles) - pieceboard.piececount.whiteSingles - pieceboard.piececount.whiteDoubles);
    return score;
    // TODO add average number of available moves per piece
};
//Update board state with selected move
void Board::doMove(const PieceBoard &new_pieceboard)
{
    pieceboard = new_pieceboard;
    turn = turn * -1;
    createPossibleBoards();
};
void Board::undoMove()
{
    //use board history
    turn = turn * -1;
    createPossibleBoards();
};
//Crown piece
void Board::crown(PieceBoard &pieceboard, const Piece &p)
{
    Piece &piece = pieceboard.piecemap.at(p.row).at(p.col);
    switch (piece.color)
    {
    case 1:
        pieceboard.piececount.whiteDoubles++;
        pieceboard.piececount.whiteSingles--;
        break;
    case -1:
        pieceboard.piececount.blackDoubles++;
        pieceboard.piececount.blackSingles--;
        break;
    };
    piece.piece = 2;
    piece.getDirection();
};
//Bear-off piece
void Board::bearOff(PieceBoard &pieceboard, const Piece &p)
{
    Piece &piece = pieceboard.piecemap.at(p.row).at(p.col);
    switch (piece.color)
    {
    case 1:
        pieceboard.piececount.whiteDoubles--;
        pieceboard.piececount.whiteSingles++;
        break;
    case -1:
        pieceboard.piececount.blackDoubles--;
        pieceboard.piececount.blackSingles++;
        break;
    };
    piece.piece = 1;
    piece.getDirection();
};
//Remove / bear-off piece based on type after impasse
void Board::remove(PieceBoard &pieceboard, const Piece &p)
{
    switch (p.piece)
    {
    case 1:
        switch (p.color)
        {
        case 1:
            pieceboard.piececount.whiteSingles--;
            break;
        case -1:
            pieceboard.piececount.blackSingles--;
            break;
        };
        pieceboard.piecemap.at(p.row).erase(p.col);
        break;
    // bear-off
    case 2:
        //bearoff
        bearOff(pieceboard, p);
        break;
    };
    // Chickendinner
    if (pieceboard.piececount.whiteSingles + pieceboard.piececount.whiteDoubles == 0)
    {
        state = 1;
    }
    else if (pieceboard.piececount.blackSingles + pieceboard.piececount.blackDoubles == 0)
    {
        state = -1;
    };
};
//Create set of current color singles
PieceSet Board::checkSingles(PieceBoard &pieceboard, const Piece &piece) const
{
    PieceSet singles;
    for (auto x : pieceboard.piecemap)
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
    return (piece.row < 7 && piece.row > 0 && (i == 1 || i == -1) && pieceboard.piecemap.count(row) > 0 && pieceboard.piecemap.at(row).count(col) > 0 && pieceboard.piecemap.at(row).at(col).color == turn && piece.piece + pieceboard.piecemap.at(row).at(col).piece == 3);
};
bool Board::ifEmptySquare(const Piece &piece, const int &row, const int &col) const
{
    return (pieceboard.piecemap.count(row) == 0 || (pieceboard.piecemap.count(row) > 0 && pieceboard.piecemap.at(row).count(col) == 0));
};
void Board::addPieceMoves(const Piece &piece)
{
    PieceMap new_piecemap = pieceboard.piecemap;
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
                Piece square = transposable ? pieceboard.piecemap.at(row).at(col) : Piece(0, 0, row, col);
                // crown
                if (row == 0 || row == 7)
                {
                    if (piece.piece == 1)
                    {
                        PieceSet singles = checkSingles(pieceboard, piece);
                        // if other single available
                        if (singles.size() > 0)
                        {
                            for (auto removepiece : singles)
                            {
                                crown(pieceboard, piece);
                                move(pieceboard, piece, square);
                                remove(pieceboard, removepiece);
                                possibleboards.push_back(pieceboard);
                            }
                        }
                        // if no other singles available, just add as normal slide (and add to crownstack)
                        else
                        {
                            move(pieceboard, piece, square);
                            pieceboard.piecetocrown[turn] = Piece(1, turn, square.row, square.col);
                            possibleboards.push_back(pieceboard);
                        }
                    }
                    else
                    {
                        bearOff(pieceboard, piece);
                        bool crowned = crownIf(pieceboard, piece);
                        if (!crowned)
                        {
                            move(pieceboard, piece, square);
                        };
                        possibleboards.push_back(pieceboard);
                    }
                }
                else
                {
                    move(pieceboard, piece, square);
                    possibleboards.push_back(pieceboard);
                };
                //if it was a transpose, break loop with current sign
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
void Board::move(PieceBoard &pieceboard, Piece piece, Piece square)
{
    Piece oldpiece = piece;
    piece.row = square.row;
    piece.col = square.col;
    square.row = oldpiece.row;
    square.col = oldpiece.col;
    pieceboard.piecemap.at(square.row).at(square.col) = piece;
    if (square.piece > 0)
    {
        pieceboard.piecemap.at(piece.row).at(piece.col) = square;
    }
    else
    {
        pieceboard.piecemap.at(piece.row).erase(piece.col);
    };
};
bool Board::crownIf(PieceBoard &pieceboard, const Piece &p)
{
    const Piece &removepiece = pieceboard.piecemap.at(p.row).at(p.col);
    bool crowned = false;
    if (pieceboard.piecetocrown.count(turn) > 0)
    {
        const Piece &crownpiece = pieceboard.piecetocrown.at(turn);
        crown(pieceboard, crownpiece);
        pieceboard.piecetocrown.erase(turn);
        remove(pieceboard, removepiece);
        crowned = true;
    }
    return crowned;
};
void Board::addImpassable()
{
    PieceBoard new_pieceboard = pieceboard;
    for (auto x : new_pieceboard.piecemap)
    {
        int row = x.first;
        for (auto y : x.second)
        {
            int col = y.first;
            Piece piece = y.second;
            if (piece.color == turn)
            {
                if (piece.piece == 2)
                {
                    remove(new_pieceboard, piece);
                    crownIf(new_pieceboard, piece);
                }
                else
                {
                    remove(new_pieceboard, piece);
                };
                possibleboards.push_back(new_pieceboard);
            };
        }
    }
};
void Board::initBoard()
{
    deleteBoard();
    pieceboard.piecemap.clear();
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            int pos = row * 8 + col;
            if (pos == 0 || pos == 4 || pos == 11 || pos == 15)
            {
                pieceboard.piecemap[row][col] = Piece(1, 1, row, col);
            }
            else if (pos == 50 || pos == 54 || pos == 57 || pos == 61)
            {
                pieceboard.piecemap[row][col] = Piece(2, 1, row, col);
            }
            else if (pos == 48 || pos == 52 || pos == 59 || pos == 63)
            {
                pieceboard.piecemap[row][col] = Piece(1, -1, row, col);
            }
            else if (pos == 2 || pos == 6 || pos == 9 || pos == 13)
            {
                pieceboard.piecemap[row][col] = Piece(2, -1, row, col);
            }
        }
    };
};
void Board::getPieceCount()
{
    for (auto x : pieceboard.piecemap)
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
                    pieceboard.piececount.whiteSingles++;
                    break;
                case -1:
                    pieceboard.piececount.blackSingles++;
                    break;
                };
                break;
            case 2:
                switch (piece.color)
                {
                case 1:
                    pieceboard.piececount.whiteDoubles++;
                    break;
                case -1:
                    pieceboard.piececount.blackDoubles++;
                    break;
                };
                break;
            };
        };
    };
};
