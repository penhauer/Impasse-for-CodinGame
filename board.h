#pragma once
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>

typedef int BoardArray[64];

typedef std::set<int> PosSet;
// Map of all possible moves at the moment

struct Piece
{
    int piece;
    int pos;
    Piece(int piece, int pos) : piece(piece), pos(pos){};
};

struct Move
{
    Piece from;
    Piece to;
    Piece remove;
    Move(Piece from, Piece to, Piece remove = Piece(0,-1)) : from(from), to(to), remove(remove){};

    bool operator<(const Move &other) const
    {
        return from.pos < other.from.pos && to.pos < other.to.pos && remove.pos < other.remove.pos;
    };
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

typedef std::unordered_map<int, MoveSet> MoveMap;

struct PosMoveSet
{
    int pos;
    MoveSet moveset;
    PosMoveSet(int pos, MoveSet moveset) : pos(pos), moveset(moveset){};
};

typedef std::vector<Move> MoveStack;

typedef std::map<int,int> PieceToCrown;

struct PieceCount {
    int whiteSingles;
    int whiteDoubles;
    int blackSingles;
    int blackDoubles;
};

class Board
{
public:
    int turn;
    int state;
    BoardArray boardarray;
    PieceCount piececount;
    MoveMap movemap;
    MoveStack movestack;
    PieceToCrown piecetocrown;
    void reset_board();
    int pieceDirection(const int &piece) const;
    void removePiece(const Piece &p);
    void changePieceType(const Piece &p);
    PosMoveSet checkPieceDiagonals(const int &pos) const;
    void makeMoveMap();
    void crownIf(const Piece &p);
    void addImpassable();
    PosSet checkSingles() const;
    void doMove(const Move &move);
    void undoMove();
    void print() const;
    void save_board() const;
    void delete_board() const;
private:
    void get_board(bool paused);
    void get_piececount();
};
