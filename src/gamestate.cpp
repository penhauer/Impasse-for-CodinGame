#include <iostream>
#include "gamestate.h"

// Simple struct to represent a move
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
        reset();
    };
    void reset()
    {
        player = 1;
        turn = 1;
        state = 0;
        board = Board();
        makeMoveMap();
    };
    int pieceDirection(const int &piece) const
    {
        return (piece == 1 || piece == -2) ? 1 : -1;
    };
    void removePiece(const Piece &p)
    {
        int &piece = board.boardarray[p.pos];
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
        int &piece = board.boardarray[p.pos];
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
        const int &piece = board.boardarray[pos];
        const int &direction = pieceDirection(piece);
        MoveSet moveset;
        // right
        for (int p = pos; 0 <= p < 64; p += direction)
        {
            const int &square = board.boardarray[pos];
            // transpose, but only if the piece is a double and square is 1
            if (p - pos == 9 * direction && piece == 2 * turn && square == turn)
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
            else if ((p - pos) % 8 == 1 * direction && square == 0)
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
        };
        // left
        for (int p = pos; 0 <= p < 64; p += direction)
        {
            const int &square = board.boardarray[pos];
            // transpose
            if (p - pos == 7 * direction && piece == 2 * turn && square == turn)
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
                break;
            }
            // slide
            else if ((p - pos) % 8 == 7 * direction && square == 0)
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
            const int &piece = board.boardarray[pos];
            if (piece * turn > 0) // if same color
            {
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
    //new single was added, so check if one needs to be crowned
    void crownIf(const Piece &p)
    {
        if (tocrown.count(turn) > 0)
        {
            const int &pos = tocrown[turn];
            const int &piece = board.boardarray[pos];
            changePieceType(Piece(piece, pos));
            tocrown.erase(turn);
            removePiece(p);
        }
    }
    void addImpassable()
    {
        for (int pos = 0; pos < 64; pos++)
        {
            const int &piece = board.boardarray[pos];
            if (piece * turn > 0)
            {
                Move move = Move(Piece(piece, pos), Piece(0, -1));
                MoveSet moveset = {move};
                movemap[pos] = moveset;
            };
        };
    };
    PosSet checkSingles() const
    {
        PosSet singles;
        for (int pos = 0; pos < 64; pos++)
        {
            const int &piece = board.boardarray[pos];
            if (piece == turn)
            {
                singles.insert(pos);
            };
        };
        return singles;
    };
    void doMove(const Move &move)
    {
        // Impasse
        if (move.from.piece == 0)
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
                tocrown[turn] = move.to.pos;
            }
        };
        int from = board.boardarray[move.from.pos];
        board.boardarray[move.from.pos] = board.boardarray[move.to.pos];
        board.boardarray[move.to.pos] = from;

        movestack.push_back(move);
        turn = turn * -1;
        makeMoveMap();
    };
    void undoMove()
    {
        Move move = movestack.back();
        movestack.pop_back();
    };
};