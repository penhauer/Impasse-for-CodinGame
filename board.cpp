#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "board.h"
#include <chrono>

Piece::Piece()
{
    piece = 0;
    color = 0;
    row = -1;
    col = -1;
};
Piece::Piece(int piece, int color, int row, int col) : piece(piece), color(color), row(row), col(col) { getDirection(); };
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
Move::Move(int from = -1, int to = -1, int remove = -1) : from(from), to(to), remove(remove){};

PieceBoard::PieceBoard(){};

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
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    createPossibleBoards();
    end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time to create possible boards: " << duration.count() << " microseconds" << std::endl;
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
        // TODO add piecetocrown and pieceset
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
// If saved board exists, delete it
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
// Print board to console
void Board::printBoard() const
{
    for (int row = 7; row >= 0; row--)
    {
        std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
        std::cout << row + 1 << " ";
        for (int col = 0; col < 8; col++)
        {
            std::cout << "|";
            if (pieceboard.piecemap.count(row) > 0 && pieceboard.piecemap.at(row).count(col) > 0)
            {
                const Piece &piece = pieceboard.piecemap.at(row).at(col);
                switch (piece.piece)
                {
                case 1:
                    switch (piece.color)
                    {
                    case 1:
                        std::cout << " ○ ";
                        break;
                    case -1:
                        std::cout << " ● ";
                        break;
                    }
                    break;
                case 2:
                    switch (piece.color)
                    {
                    case 1:
                        std::cout << " ◇ ";
                        break;
                    case -1:
                        std::cout << " ◆ ";
                        break;
                    }
                    break;
                };
            }
            else if ((row + col) % 2 != 0)
            {
                std::cout << " / ";
            }
            else
            {
                std::cout << "   ";
            };
        };
        std::cout << "|" << std::endl;
    };
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    std::cout << "    A   B   C   D   E   F   G   H " << std::endl;
};
// Print current valid moves to console
void Board::printMove(const Move &move) const
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
// Create current valid moves
void Board::createPossibleBoards()
{
    possiblepieceboards.clear();
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
    if (possiblepieceboards.size() == 0)
    {
        // if no move, impasse
        addImpassable();
    };
};
// Evaluate heuristic value of current board
int Board::evaluate(int color) const
{
    // return color * (3*(float)(pieceboard.piececount.blackDoubles-pieceboard.piececount.whiteDoubles) +2*(pieceboard.piececount.blackSingles-pieceboard.piececount.whiteSingles));
    //  TODO add average number of available moves per piece
    return color * (2 * pieceboard.piececount.blackSingles + 3 * pieceboard.piececount.blackDoubles - 2 * pieceboard.piececount.whiteSingles - 3 * pieceboard.piececount.whiteDoubles);
};
// Update board state with selected move
void Board::doMove(const PieceBoard new_pieceboard)
{
    pieceboardhistory.push_back(pieceboard);
    pieceboard = new_pieceboard;
    turn = turn * -1;
};
void Board::undoMove()
{
    pieceboard = pieceboardhistory.back();
    pieceboardhistory.pop_back();
    turn = turn * -1;
};
// Crown piece
void Board::crown(PieceBoard &pieceboard, const Piece &p)
{
    Piece &piece = pieceboard.piecemap[p.row][p.col];
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
// Bear-off piece
void Board::bearOff(PieceBoard &pieceboard, const Piece &p)
{
    Piece &piece = pieceboard.piecemap[p.row][p.col];
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
// Remove / bear-off piece based on type after impasse
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
        // bearoff
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
// Create set of current color singles
PieceSet Board::checkSingles(const Piece &piece) const
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
    // to cover for left and right search
    for (int sign = 0; sign < 2; sign++)
    {
        int i = piece.direction;
        while (i + piece.row < 8 && piece.row + i >= 0)
        {
            int row = piece.row + i;
            int col = piece.col + i * pow(-1, sign);
            if (col < 0 || col > 7)
            {
                break;
            };
            // slide or transpose
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
                        PieceSet singles = checkSingles(piece);
                        // if other single available
                        if (singles.size() > 0)
                        {
                            for (auto removepiece : singles)
                            {
                                PieceBoard new_pieceboard = pieceboard;
                                new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, row * 8 + col, removepiece.row * 8 + removepiece.col);
                                crown(new_pieceboard, piece);
                                move(new_pieceboard, piece, square);
                                remove(new_pieceboard, removepiece);
                                possiblepieceboards.push_back(new_pieceboard);
                            }
                        }
                        // if no other singles available, just add as normal slide (and add to crownstack)
                        else
                        {
                            PieceBoard new_pieceboard = pieceboard;
                            new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, row * 8 + col);
                            move(new_pieceboard, piece, square);
                            new_pieceboard.piecetocrown[turn] = Piece(1, turn, square.row, square.col);
                            possiblepieceboards.push_back(new_pieceboard);
                        }
                    }
                    else
                    {
                        PieceBoard new_pieceboard = pieceboard;
                        bearOff(new_pieceboard, piece);
                        bool crowned = crownIf(new_pieceboard, piece);
                        if (!crowned)
                        {
                            new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, row * 8 + col, piece.row * 8 + piece.col);
                            move(new_pieceboard, piece, square);
                        }
                        else
                        {
                            new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, row * 8 + col);
                        };
                        possiblepieceboards.push_back(new_pieceboard);
                    }
                }
                else
                {
                    PieceBoard new_pieceboard = pieceboard;
                    new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, row * 8 + col);
                    move(new_pieceboard, piece, square);
                    possiblepieceboards.push_back(new_pieceboard);
                };
                // if it was a transpose, break loop with current sign
                if (transposable)
                {
                    break;
                };
            }
            // if the square was not empty or transposable, break
            else
            {
                break;
            };
            // otherwise increment i and continue loop
            i = i + piece.direction;
        };
    };
};
void Board::move(PieceBoard &pieceboard, Piece piece, Piece square)
{
    Piece oldpiece = pieceboard.piecemap.at(piece.row).at(piece.col);
    piece = oldpiece;
    piece.row = square.row;
    piece.col = square.col;
    pieceboard.piecemap[piece.row][piece.col] = piece;
    if (square.piece > 0)
    {
        square.row = oldpiece.row;
        square.col = oldpiece.col;
        pieceboard.piecemap[square.row][square.col] = square;
    }
    else
    {
        pieceboard.piecemap.at(oldpiece.row).erase(oldpiece.col);
    };
};
bool Board::crownIf(PieceBoard &pieceboard, const Piece &p)
{
    bool crowned = false;
    if (pieceboard.piecetocrown.count(turn) > 0)
    {
        const Piece &removepiece = pieceboard.piecemap.at(p.row).at(p.col);
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
    for (auto x : pieceboard.piecemap)
    {
        int row = x.first;
        for (auto y : x.second)
        {
            int col = y.first;
            Piece piece = y.second;
            if (piece.color == turn)
            {
                PieceBoard new_pieceboard = pieceboard;
                if (piece.piece == 2)
                {
                    if ((row == 0 || row == 7) && pieceboard.piecetocrown.count(turn) > 0)
                    {
                        Piece &toremove = pieceboard.piecetocrown[turn];
                        remove(new_pieceboard, toremove);
                        new_pieceboard.piecetocrown.erase(turn);
                        crown(new_pieceboard, piece);
                        new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, -1, toremove.row * 8 + toremove.col);
                        possiblepieceboards.push_back(new_pieceboard);
                        new_pieceboard = pieceboard;
                    };
                    remove(new_pieceboard, piece);
                    crownIf(new_pieceboard, piece);
                    new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, -1, piece.row * 8 + piece.col);
                }
                else
                {
                    remove(new_pieceboard, piece);
                    new_pieceboard.lastmove = Move(piece.row * 8 + piece.col, -1, piece.row * 8 + piece.col);
                };
                possiblepieceboards.push_back(new_pieceboard);
            };
        };
    };
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
