#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "board.h"

typedef int BoardArray[64];

typedef std::set<int> PosSet;

Piece::Piece() {};
Piece::Piece(int piece, int pos) : piece(piece), pos(pos) {};

Move::Move() {};
Move::Move(Piece from, Piece to, Piece remove = Piece(-99, -99)) : from(from), to(to), remove(remove) {};
bool Move::validMove() const
    {
        return from.pos >= 0 && to.pos >= 0;
    };

namespace std {
    template <>
    struct hash<Move>
    {
        std::size_t operator()(const Move& k) const
        {
            using std::size_t;
            using std::hash;
            using std::string;
            return ((hash<int>()(k.from.piece)
                ^ (hash<int>()(k.from.pos) << 1)) >> 1)
                ^ (hash<int>()(k.to.piece)
                    ^ (hash<int>()(k.to.pos) << 1)) >> 1;
        }
    };
};

typedef std::set<Move> MoveSet;

typedef std::vector<BoardArray> BoardHistory;

typedef std::map<int, int> PieceToCrown;

Board::Board() {};
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
    updateMoveSet();
    boolPieceToCrown = false;
    piecetocrown.clear();
};
void Board::restoreLast(const Board& b)
{
    //TODO
};
void Board::saveBoard() const
{
    std::ofstream newfile;
    newfile.open("boardstate.txt", std::ios::out);
    if (newfile.is_open())
    {
        for (int i = 0; i < 64; i++)
        {
            newfile << boardarray[i] << std::endl;
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
        for (int i = 0; i < 64; i++)
        {
            if (i / 8 == row)
            {
                std::cout << boardarray[i] << " ";
            };
        };
        std::cout << std::endl;
    };
};
void Board::printMoves() const
{
    //TODO implement
};
int Board::pieceDirection(const int& piece) const
{
    return (piece == 1 || piece == -2) ? 1 : -1;
};
void Board::updateMoveSet()
{
    moveset.clear();
    // return array of possible moves
    for (int pos = 0; pos < 64; pos++)
    {
        const int& piece = boardarray[pos];
        if (piece * turn > 0) // if same color
        {
            addPieceDiagonals(pos);
        };
    };
    // if no move, impasse
    if (moveset.size() == 0)
    {
        addImpassable();
    };
};
void Board::doMove(const Move& move)
{

    // Impasse
    if (move.from.pos == move.remove.pos)
    {
        if (move.remove.piece % 2 == 0)
        {
            changePieceType(move.from);
            crownIf(Piece(1, move.from.pos));
        }
        else
        {
            removePiece(move.from);
        };
    }
    // bear off and crown
    else if (move.to.pos < 8 || move.to.pos > 55)
    {
        //bear-off
        if (move.from.piece % 2 == 0)
        {
            changePieceType(move.from);
            crownIf(move.from);
        }
        //crown if single was available
        else if (move.remove.piece != 0)
        {
            changePieceType(move.from);
            removePiece(move.remove);
        }
        //otherwise put crownable to stack
        else
        {
            piecetocrown[turn] = move.to.pos;
            boolPieceToCrown = true;
        }
    };
    int from = boardarray[move.from.pos];
    boardarray[move.from.pos] = boardarray[move.to.pos];
    boardarray[move.to.pos] = from;

    //boardhistory.push_back(move); TODO
    turn = turn * -1;
    updateMoveSet();
};
void Board::undoMove()
{
    //Move move = boardhistory.back();
    //boardhistory.pop_back();
};
void Board::changePieceType(const Piece& p)
{
    int& piece = boardarray[p.pos];
    switch (piece)
    {
    case 1:
        piece = 2;
        piececount.whiteSingles--;
        piececount.whiteDoubles++;
        break;
    case 2:
        piece = 1;
        piececount.whiteSingles++;
        piececount.whiteDoubles--;
        break;
    case -1:
        piece = -2;
        piececount.blackSingles--;
        piececount.blackDoubles++;
        break;
    case -2:
        piece = -1;
        piececount.blackSingles++;
        piececount.blackDoubles--;
        break;
    };
};
void Board::removePiece(const Piece& p)
{
    int& piece = boardarray[p.pos];
    switch (piece)
    {
    case 1:
        piececount.whiteSingles--;
        break;
    case 2:
        piececount.whiteDoubles--;
        break;
    case -1:
        piececount.blackSingles--;
        break;
    case -2:
        piececount.blackDoubles--;
        break;
    };
    piece = 0;
    // chickendinner
    if (piececount.whiteSingles + piececount.whiteDoubles == 0)
    {
        state = 1;
    }
    else if (piececount.blackSingles + piececount.blackDoubles == 0)
    {
        state = -1;
    }
};
PosSet Board::checkSingles() const
{
    PosSet singles;
    for (int pos = 0; pos < 64; pos++)
    {
        const int& piece = boardarray[pos];
        if (piece == turn)
        {
            singles.insert(pos);
        };
    };
    return singles;
};
void Board::addPieceDiagonals(const int& pos)
{
    const int& piece = boardarray[pos];
    const int& direction = pieceDirection(piece);
    // right
    for (int p = pos; p < 64 && p >= 0; p += direction)
    {
        const int& square = boardarray[pos];
        // transpose, but only if the piece is a double and square is 1
        if (p - pos == 9 * direction && piece == 2 * turn && square == turn)
        {
            // if crowning
            if (piece % 2 == 1 && (p > 55 || p < 8))
            {
                std::set<int> singles = checkSingles();
                for (auto& s : singles) // TODO what if empty
                {
                    Move move = Move(Piece(piece, pos), Piece(square, p), Piece(turn, s));
                    moveset.insert(move);
                }
            }
            else
            {
                Move move = Move(Piece(piece, pos), Piece(square, p));
                moveset.insert(move);
            }
            break;
        }
        // slide
        else if ((p - pos) % 8 == 1 * direction && square == 0)
        {
            if (piece % 2 == 1 && (p > 55 || p < 8))
            {
                std::set<int> singles = checkSingles();
                for (auto& s : singles) // TODO what if empty
                {
                    Move move = Move(Piece(piece, pos), Piece(square, p), Piece(turn, s));
                    moveset.insert(move);
                }
            }
            else
            {
                Move move = Move(Piece(piece, pos), Piece(square, p));
                moveset.insert(move);
            }
        }
        else
        {
            break;
        }
    };
    // left
    for (int p = pos; p < 64 && p >= 0; p += direction)
    {
        const int& square = boardarray[pos];
        // transpose
        if (p - pos == 7 * direction && piece == 2 * turn && square == turn)
        {
            if (piece % 2 == 1 && (p > 55 || p < 8))
            {
                std::set<int> singles = checkSingles();
                for (auto& s : singles) // TODO what if empty
                {
                    Move move = Move(Piece(piece, pos), Piece(square, p), Piece(turn, s));
                    moveset.insert(move);
                }
            }
            else
            {
                Move move = Move(Piece(piece, pos), Piece(square, p));
                moveset.insert(move);
            }
            break;
        }
        // slide
        else if ((p - pos) % 8 == 7 * direction && square == 0)
        {
            if (piece % 2 == 1 && (p > 55 || p < 8))
            {
                std::set<int> singles = checkSingles();
                for (auto& s : singles) // TODO what if empty
                {
                    Move move = Move(Piece(piece, pos), Piece(square, p), Piece(turn, s));
                    moveset.insert(move);
                }
            }
            else
            {
                Move move = Move(Piece(piece, pos), Piece(square, p));
                moveset.insert(move);
            }
        }
        else
        {
            break;
        }
    };
};
void Board::crownIf(const Piece& p)
{
    if (boolPieceToCrown)
    {
        const int& pos = piecetocrown[turn];
        const int& piece = boardarray[pos];
        changePieceType(Piece(piece, pos));
        piecetocrown.erase(turn);
        boolPieceToCrown = false;
        removePiece(p);
    }
};
void Board::addImpassable()
{
    for (int pos = 0; pos < 64; pos++)
    {
        const int& piece = boardarray[pos];
        if (piece * turn > 0)
        {
            Move move = Move(Piece(piece, pos), Piece(0, -1));
            moveset.insert(move);
        };
    };
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
                getline(newfile, tp);
                boardarray[i] = std::stoi(tp);
            }
            newfile.close();
        };
    }
    else
    {
        //deleteBoard(); TODO
        for (int pos = 0; pos < 64; pos++)
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
    };
};
void Board::getPieceCount()
{
    for (int pos = 0; pos < 64; pos++)
    {
        const int& piece = boardarray[pos];
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
};
void Board::getIfToCrown()
{
    //TODO for when setting up the board again, or just save the whole boardstate
}