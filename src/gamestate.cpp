#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <vector>
#include "board.h"

struct Piece
{
    int piece;
    int pos;
    Piece(int piece, int pos) : piece(piece), pos(pos){};
};

// Simple struct to represent a move
struct Move
{
    Piece from;
    Piece to;
    int type; // 0 = slide, 1 = crown, 2 = impasse
    Piece remove;
    Move(Piece from, Piece to, int type, Piece remove = Piece(0,-1)) : from(from), to(to), type(type), remove(remove){};
};

// Map of all possible moves at the moment
typedef std::set<Move> MoveSet;
typedef std::unordered_map<int, MoveSet> MoveMap;
struct PosMoveSet
{
    int pos;
    MoveSet moveset;
    PosMoveSet(int pos, MoveSet moveset) : pos(pos), moveset(moveset){};
};

typedef std::vector<Move> MoveStack;

typedef std::map<int,int> ToCrown;

class GameState
{
public:
    int player; // 0 = Ai vs Ai, 1 = White vs Ai, -1 = Black vs Ai, 2 = White vs Black
    int turn;   // 1 = white, -1 = black
    int state;  // 0 = in progress, 1 = white win, -1 = black win
    Board board;
    MoveMap movemap;
    MoveStack movestack;
    ToCrown tocrown;
    GameState()
    {
        player = 1;
        turn = 1;
        state = 0;
        board = Board();
    };
    int pieceDirection(const int &piece) const
    {
        return (piece == 1 || piece == -2) ? 1 : -1;
    };
    void movePiece(const Move &move)
    {
        if (move.to.pos < 8 || move.to.pos > 55)
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
                changePieceType(move.from);
                tocrown[turn] = move.to.pos;
            }
        };
        int from = board.boardmap[move.from.pos];
        board.boardmap[move.from.pos] = board.boardmap[move.to.pos];
        board.boardmap[move.to.pos] = from;
    };
    void removePiece(const Piece &p)
    {
        int &piece = board[p.pos];
        switch (piece)
        {
        case 1:
            board.piececount.whiteSingles--;
            break;
        case 2:
            board.piececount.whiteDoubles--;
            break;
        case -1:
            board.piececount.blackSingles--;
            break;
        case -2:
            board.piececount.blackDoubles--;
            break;
        };
        piece = 0;
        // chickendinner
        if (board.piececount.whiteSingles + board.piececount.whiteDoubles == 0)
        {
            state = 1;
        }
        else if (board.piececount.blackSingles + board.piececount.blackDoubles == 0)
        {
            state = -1;
        }
    };
    void changePieceType(const Piece &p)
    {
        int &piece = board.at(p.pos);
        switch (piece)
        {
        case 1:
            piece = 2;
            board.piececount.whiteSingles--;
            board.piececount.whiteDoubles++;
            break;
        case 2:
            piece = 1;
            board.piececount.whiteSingles++;
            board.piececount.whiteDoubles--;
            break;
        case -1:
            piece = -2;
            board.piececount.blackSingles--;
            board.piececount.blackDoubles++;
            break;
        case -2:
            piece = -1;
            board.piececount.blackSingles++;
            board.piececount.blackDoubles--;
            break;
        };
    };
    PosMoveSet checkPieceDiagonals(const int &pos) const
    {
        // check diagonals forward
        const int &piece = board.at(pos);
        const int &direction = pieceDirection(piece);
        MoveSet moveset;
        // right
        for (int p = pos; 0 <= p < 64; p += direction)
        {
            const int &square = board.at(pos);
            // transpose
            if (p - pos == 9 * direction && (square + piece) % 3 == 0)
            {
                Move move = Move(Piece(piece, pos), Piece(square, p), 1);
                moveset.insert(move);
                break;
            }
            // slide
            else if ((p - pos) % 8 == 1 * direction && square == 0)
            {
                Move move = Move(Piece(piece, pos), Piece(square, p), 0);
                moveset.insert(move);
            }
            else
            {
                break;
            }
        };
        // left
        for (int p = pos; 0 <= p < 64; p += direction)
        {
            const int &square = board.at(pos);
            // transpose
            if (p - pos == 7 * direction && (square + piece) % 3 == 0)
            {
                Move move = Move(Piece(piece, pos), Piece(square, p), 1);
                moveset.insert(move);
                break;
            }
            // slide
            else if ((p - pos) % 8 == 7 * direction && square == 0)
            {
                Move move = Move(Piece(piece, pos), Piece(square, p), 0);
                moveset.insert(move);
            }
            else
            {
                break;
            }
        };
        PosMoveSet posmoveset = PosMoveSet(pos, moveset);
        return posmoveset;
    };
    void makeMoveMap()
    {
        movemap.clear();
        // return array of possible moves
        for (int pos = 0; pos < 64; pos++)
        {
            const int &piece = board.at(pos);
            if (piece * turn > 0) // if same color
            {
                //check if removable
                PosMoveSet posmoveset = checkPieceDiagonals(pos);
                if (posmoveset.moveset.size() > 0)
                {
                    movemap[posmoveset.pos] = posmoveset.moveset;
                }
            };
        };
        // if no move, impasse
        if (movemap.size() == 0)
        {
            addImpassable();
        };
    };
    void moveImpasse(const Move &move)
    {
        if (move.from.piece % 2 == 0)
        {
            changePieceType(move.from);
            crownIf(Piece(1, move.to.pos));
        }
        else
        {
            removePiece(move.from);
        }
    };
    void crownIf(const Piece &p)
    {
        if (tocrown.count(turn) > 0)
        {
            const int &pos = tocrown[turn];
            const int &piece = board.at(pos);
            changePieceType(Piece(piece, pos));
            tocrown.erase(turn);
            removePiece(p);
        }
    }
    void addImpassable()
    {
        for (int pos = 0; pos < 64; pos++)
        {
            const int &piece = board.at(pos);
            if (piece * turn > 0)
            {
                Move move = Move(Piece(piece, pos), Piece(0, -1), 2);
                MoveSet moveset = {move};
                movemap[pos] = moveset;
            };
        };
    };
    void doMove(const Move &move)
    {
        if (move.type == 2) //impasse
        {
            moveImpasse(move);
        }
        else // slide or transpose
        {
            movePiece(move);
        }
    movestack.push_back(move);
    turn = turn * -1;
    makeMoveMap();
    };
    void undoMove()
    {
        Move move = movestack.back();
        movestack.pop_back();
        //TODO implement
    };
};