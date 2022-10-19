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
};
Piece::Piece(int piece, int color) : piece(piece), color(color) { getDirection(); };
void Piece::getDirection() { direction = (piece == 1 && color == 1 || piece == 2 && color == -1) ? 1 : -1; };
void Piece::changeType()
{
    piece = (piece == 2) ? 1 : 2;
    getDirection();
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
    pieceboard.postocrown.clear();
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
        newfile << pieceboard.postocrown.size() << std::endl;
        // TODO add postocrown and pieceset
    };
    newfile.close();
};
void Board::loadBoard(int turn, PosToCrown postocrown, PosSet pieceset)
{
    state = 0;
    this->turn = turn;
    this->pieceboard.postocrown = postocrown;
    /*for (Piece piece : pieceset)
    {
        pieceboard.piecemap[piece.row][piece.col] = piece;
    };
    */ //TODO fix
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
            int pos = row * 8 + col;
            if (pieceboard.piecemap.count(pos) > 0)
            {
                const Piece &piece = pieceboard.piecemap.at(pos);
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
        const int &pos = x.first;
        const Piece &piece = x.second;
        if (piece.color == turn)
        {
            addPieceMoves(piece, pos);
        }
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
void Board::crown(PieceBoard &pieceboard, const Piece &p, const int &pos)
{
    Piece &piece = pieceboard.piecemap[pos];
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
void Board::bearOff(PieceBoard &pieceboard, const Piece &p, const int &pos)
{
    Piece &piece = pieceboard.piecemap[pos];
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
void Board::remove(PieceBoard &pieceboard, const Piece &p, const int &pos)
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
        pieceboard.piecemap.erase(pos);
        break;
    // bear-off
    case 2:
        // bearoff
        bearOff(pieceboard, p, pos);
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
PosSet Board::checkSingles(const Piece &piece, const int &pos) const
{
    PosSet singles;
    for (auto x : pieceboard.piecemap)
    {
        const int &ppos = x.first;
        Piece p = x.second;
        if (p.piece == 1 && p.color == turn && (ppos != pos))
        {
            singles.insert(ppos);
        };
    };
    return singles;
};
bool Board::ifTransposable(const Piece &piece, const int &piecerow, const int &i, const int &topos) const
{
    return (piecerow < 7 && piecerow > 0 && (i == 1 || i == -1) && pieceboard.piecemap.count(topos) > 0 && pieceboard.piecemap.at(topos).color == turn && piece.piece + pieceboard.piecemap.at(topos).piece == 3);
};
bool Board::ifEmptySquare(const int &pos) const
{
    return pieceboard.piecemap.count(pos) == 0;
};
void Board::addPieceMoves(const Piece &piece, const int &pos)
{
    int piecerow = pos / 8;
    int piececol = pos % 8;
    // to cover for left and right search
    for (int sign = 0; sign < 2; sign++)
    {
        int i = piece.direction;
        while (piecerow + i < 8 && piecerow + i >= 0)
        {
            int row = piecerow + i;
            int col = piececol + i * pow(-1, sign);
            int topos = row * 8 + col;
            if (col < 0 || col > 7)
            {
                break;
            };
            // slide or transpose
            bool transposable = ifTransposable(piece, piecerow, i, topos);
            bool emptysquare = ifEmptySquare(topos);
            if (emptysquare || transposable)
            {
                Piece square = transposable ? pieceboard.piecemap.at(topos) : Piece(0, 0);
                // crown
                if (row == 0 || row == 7)
                {
                    if (piece.piece == 1)
                    {
                        PosSet singles = checkSingles(piece, pos);
                        // if other single available
                        if (singles.size() > 0)
                        {
                            for (auto removepiecepos : singles)
                            {
                                PieceBoard new_pieceboard = pieceboard;
                                Piece &removepiece = new_pieceboard.piecemap.at(removepiecepos);
                                new_pieceboard.lastmove = Move(pos, topos, removepiecepos);
                                crown(new_pieceboard, piece, pos);
                                move(new_pieceboard, pos, topos);
                                remove(new_pieceboard, removepiece, removepiecepos);
                                possiblepieceboards.push_back(new_pieceboard);
                            }
                        }
                        // if no other singles available, just add as normal slide (and add to crownstack)
                        else
                        {
                            PieceBoard new_pieceboard = pieceboard;
                            new_pieceboard.lastmove = Move(pos, topos);
                            move(new_pieceboard, pos, topos);
                            new_pieceboard.postocrown[turn] = topos;
                            possiblepieceboards.push_back(new_pieceboard);
                        }
                    }
                    else
                    {
                        PieceBoard new_pieceboard = pieceboard;
                        bearOff(new_pieceboard, piece, pos);
                        bool crowned = crownIf(new_pieceboard, piece, pos);
                        if (!crowned)
                        {
                            new_pieceboard.lastmove = Move(pos, topos, pos);
                            move(new_pieceboard, pos, topos);
                        }
                        else
                        {
                            new_pieceboard.lastmove = Move(pos, topos, -1);
                        };
                        possiblepieceboards.push_back(new_pieceboard);
                    }
                }
                else
                {
                    PieceBoard new_pieceboard = pieceboard;
                    new_pieceboard.lastmove = Move(pos, topos, -1);
                    move(new_pieceboard, pos, topos);
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
void Board::move(PieceBoard &pieceboard, int pos, int topos)
{

    if (pieceboard.piecemap.count(topos) > 0)
    {
        Piece topiece = pieceboard.piecemap[topos];
        pieceboard.piecemap[topos] = pieceboard.piecemap.at(pos);
        pieceboard.piecemap[pos] = topiece;
    }
    else
    {
        pieceboard.piecemap[topos] = pieceboard.piecemap.at(pos);
        pieceboard.piecemap.erase(pos);
    }
};
bool Board::crownIf(PieceBoard &pieceboard, const Piece &p, const int &pos)
{
    bool crowned = false;
    if (pieceboard.postocrown.count(turn) > 0)
    {
        const Piece &removepiece = pieceboard.piecemap.at(pos);
        const int &crownpiecepos = pieceboard.postocrown.at(turn);
        const Piece &crownpiece = pieceboard.piecemap.at(crownpiecepos);
        crown(pieceboard, crownpiece, crownpiecepos);
        pieceboard.postocrown.erase(turn);
        remove(pieceboard, removepiece, pos);
        crowned = true;
    }
    return crowned;
};
void Board::addImpassable()
{
    for (auto x : pieceboard.piecemap)
    {
        int pos = x.first;
        Piece piece = x.second;
        int row = pos / 8;
        int col = pos % 8;
        if (piece.color == turn)
        {
            PieceBoard new_pieceboard = pieceboard;
            if (piece.piece == 2)
            {
                if ((row == 0 || row == 7) && pieceboard.postocrown.count(turn) > 0)
                {
                    int &removepiecepos = pieceboard.postocrown.at(turn);
                    Piece &removepiece = pieceboard.piecemap.at(removepiecepos);
                    remove(new_pieceboard, removepiece, removepiecepos);
                    new_pieceboard.postocrown.erase(turn);
                    crown(new_pieceboard, piece, pos);
                    new_pieceboard.lastmove = Move(pos, -1, removepiecepos);
                    possiblepieceboards.push_back(new_pieceboard);
                    new_pieceboard = pieceboard;
                };
                remove(new_pieceboard, piece, pos);
                crownIf(new_pieceboard, piece, pos);
                new_pieceboard.lastmove = Move(pos, -1, pos);
            }
            else
            {
                remove(new_pieceboard, piece, pos);
                new_pieceboard.lastmove = Move(pos, -1, pos);
            };
            possiblepieceboards.push_back(new_pieceboard);
        };
    };
};
void Board::initBoard()
{
    deleteBoard();
    pieceboard.piecemap.clear();
    for (int pos = 0; pos < 64; pos++)
    {
        if (pos == 0 || pos == 4 || pos == 11 || pos == 15)
        {
            pieceboard.piecemap[pos] = Piece(1, 1);
        }
        else if (pos == 50 || pos == 54 || pos == 57 || pos == 61)
        {
            pieceboard.piecemap[pos] = Piece(2, 1);
        }
        else if (pos == 48 || pos == 52 || pos == 59 || pos == 63)
        {
            pieceboard.piecemap[pos] = Piece(1, -1);
        }
        else if (pos == 2 || pos == 6 || pos == 9 || pos == 13)
        {
            pieceboard.piecemap[pos] = Piece(2, -1);
        }
    };
};
void Board::getPieceCount()
{
    for (auto x : pieceboard.piecemap)
    {
        const Piece &piece = x.second;
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
