#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "board.h"

// pos = bsol*9 - sol*7 + 21
Piece::Piece(){};
Piece::Piece(int piece, int sol, int bsol) : piece(piece), sol(sol), bsol(bsol) { getPos(); getDirection(); };
void Piece::getPos() {pos = bsol * 9 - sol * 7 + 21;};
void Piece::getDirection() {direction = (piece == 1 || piece == -2) ? 1 : -1;};
void Piece::changeType() {piece = (piece % 2 == 0) ? piece / 2 : piece * 2; ; getDirection();};
void Piece::changePos(int sol, int bsol) {this->sol = sol; this->bsol = bsol; getPos();};

Move::Move(){};
Move::Move(Piece from, Piece to, Piece remove = Piece(-99, -99, -99)) : from(from), to(to), remove(remove){};
bool Move::validMove() const
{
    return from.pos >= 0 && to.pos >= 0;
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
            return ((hash<int>()(k.from.piece) ^ (hash<int>()(k.from.pos) << 1)) >> 1) ^ (hash<int>()(k.to.piece) ^ (hash<int>()(k.to.pos) << 1)) >> 1;
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
        std::cout << row + 1 << " ";
        for (int i = 0; i < 64; i++)
        {
            if (i / 8 == row)
            {
                const int &piece = boardarray[i];
                if (piece == 0)
                {
                    std::cout << "  ";
                }
                else if (piece == 1)
                {
                    std::cout << "○ ";
                }
                else if (piece == 2)
                {
                    std::cout << "◇ ";
                }
                else if (piece == -1)
                {
                    std::cout << "● ";
                }
                else if (piece == -2)
                {
                    std::cout << "◆ ";
                }
            };
        };
        std::cout << std::endl;
    };
    std::cout << "  A B C D E F G H" << std::endl;
};
void Board::printMoves() const {
    for (auto &move : moveset) {
        std::cout << "from: " << move.from.pos << " to: " << move.to.pos << " remove: " << move.remove.pos << std::endl;
    }
};
void Board::createMoveSet()
{
    moveset.clear();
    // return array of possible moves
    for (auto const& [sol, val] : piecemap)
    {
        for (auto const& [bsol, piece] : val)
        {
            const Piece &p = piecemap[sol][bsol];
            if (p.piece  * turn > 0)
            {
                addPieceDiagonals(p);
            }
        }
    };
    // if no move, impasse
    if (moveset.size() == 0)
    {
        addImpassable();
    };
};
void Board::doMove(const Move &move)
{

    // Impasse
    if (move.from.pos == move.remove.pos)
    {
        if (move.remove.piece % 2 == 0)
        {
            changePieceType(move.from);
            crownIf(Piece(1, move.from.sol, move.from.bsol));
        }
        else
        {
            removePiece(move.from);
        };
    }
    // bear off and crown
    else if (move.to.pos < 8 || move.to.pos > 55)
    {
        // bear-off
        if (move.from.piece % 2 == 0)
        {
            changePieceType(move.from);
            crownIf(move.from);
        }
        // crown if single was available
        else if (move.remove.piece != 0)
        {
            changePieceType(move.from);
            removePiece(move.remove);
        }
        // otherwise put crownable to stack
        else
        {
            piecetocrown[turn] = move.to;
            boolPieceToCrown = true;
        }
    };
    int from = boardarray[move.from.pos];
    boardarray[move.from.pos] = boardarray[move.to.pos];
    boardarray[move.to.pos] = from;

    // boardhistory.push_back(move); TODO
    turn = turn * -1;
    createMoveSet(); //updateMoveSet();
};
void Board::undoMove(){
    // Move move = boardhistory.back();
    // boardhistory.pop_back();
};
void Board::changePieceType(const Piece &p)
{
    piecemap[p.sol][p.bsol].changeType();
    int &piece = boardarray[p.pos];
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
void Board::removePiece(const Piece &p)
{
    piecemap.at(p.sol).erase(p.bsol);
    int &piece = boardarray[p.pos];
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
PieceSet Board::checkSingles() const
{
    PieceSet singles;
    for (int sol = 0; sol < 6; sol++)
    {
        for (int bsol = 0; bsol < 7; bsol++)
        {
            if (piecemap.at(sol).at(bsol).piece == turn)
            {
                singles.insert(piecemap.at(sol).at(bsol));
            }
        }
    };
    return singles;
};
void Board::addPieceDiagonals(const Piece &p)
{
    //check solidus and back solidus for possible moves
    int sol = p.sol; //0-6
    int bsol = p.bsol; //0-7
    int direction = p.direction;
    for (int s = sol+direction; s < 7; s += direction)


    /*
    {
        const int &square = boardarray[p];
        // transpose, but only if the piece is a double and square is 1
        if ((p - pos % 9 == 0 || p - pos % 7 == 0) && piece == 2 * turn && square == turn)
        {
            // if crowning
            if (piece % 2 == 1 && (p > 55 || p < 8))
            {
                std::set<int> singles = checkSingles();
                for (auto &s : singles) // TODO what if empty
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
        else if ((((p - pos) % 7 == 0) && ((pos % 8 != 7 && pos > p) || (pos % 8 != 0 && p > pos))) || ((p - pos) % 9 == 0 && ((pos % 8 != 7 && p > pos) || (pos % 8 == 0 && p < pos))))
        {
            if (square == 0)
            {
                if (piece % 2 == 1 && (p > 55 || p < 8))
                {
                    std::set<int> singles = checkSingles();
                    for (auto &s : singles) // TODO what if empty
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
        }
        else
        {
            continue;
        }
    };
    */
};
void Board::updateMoveSet(const Move &move)
{
    // TODO
};
void Board::crownIf(const Piece &p)
{
    if (boolPieceToCrown)
    {
        const Piece &piecetc = piecetocrown[turn];
        changePieceType(piecemap[piecetc.sol][piecetc.bsol]);
        piecetocrown.erase(turn);
        boolPieceToCrown = false;
        removePiece(p);
    }
};
void Board::addImpassable()
{
    for (int pos = 0; pos < 64; pos++)
    {
        const int &piece = boardarray[pos];
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
        // deleteBoard(); TODO
        /*for (int pos = 0; pos < 64; pos++)
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
        */
        for (int sol = 0; sol < 7; sol++)
        {
            for (int bsol = 0; bsol < 8; bsol++)
            {
                if ((sol == 3 && bsol == 0) || (sol == 4 && bsol == 2) || (sol == 5 && bsol == 2) || (sol == 6 && bsol == 4))
                {
                    piecemap[sol][bsol] = Piece(1, sol, bsol);
                    boardarray[bsol*9-sol*7+21] = 1;
                }
                else if ((sol == 0 && bsol == 4) || (sol == 1 && bsol == 4) || (sol == 6 && bsol == 2) || (sol == 6 && bsol == 3))
                {
                    piecemap[sol][bsol] = Piece(2, sol, bsol);
                    boardarray[bsol*9-sol*7+21] = 2;
                }
                else if ((sol == 0 && bsol == 3) || (sol == 1 && bsol == 5) || (sol == 2 && bsol == 5) || (sol == 3 && bsol == 7))
                {
                    piecemap[sol][bsol] = Piece(-1, sol, bsol);
                    boardarray[bsol*9-sol*7+21] = -1;
                }
                else if ((sol == 3 && bsol == 1) || (sol == 4 && bsol == 1) || (sol == 5 && bsol == 3) || (sol == 6 && bsol == 3))
                {
                    piecemap[sol][bsol] = Piece(-2, sol, bsol);
                    boardarray[bsol*9-sol*7+21] = -2;
                }
            }
        }
    };
};
void Board::getPieceCount()
{
    for (int pos = 0; pos < 64; pos++)
    {
        const int &piece = boardarray[pos];
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
    // TODO for when setting up the board again, or just save the whole boardstate
}