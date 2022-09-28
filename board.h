#pragma once

typedef BoardArray;
typedef PosSet;

struct Piece
{
    int piece;
    int pos;
    Piece(int piece, int pos);
};

struct Move
{
    Piece from;
    Piece to;
    Piece remove;
    Move(Piece from, Piece to, Piece remove = Piece(0,-1));
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

typedef MoveSet;

typedef BoardHistory;

typedef PieceToCrown;

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
    MoveSet moveset;
    BoardHistory boardhistory;
    PieceToCrown piecetocrown;
    void resetBoard();
    int pieceDirection(const int &piece) const;
    void removePiece(const Piece &p);
    void changePieceType(const Piece &p);
    void addPieceDiagonals(const int &pos);
    void updateMoveSet();
    void crownIf(const Piece &p);
    void addImpassable();
    PosSet checkSingles() const;
    void doMove(const Move &move);
    void undoMove();
    void printMoves() const;
    void printBoard() const;
    void saveBoard() const;
    void deleteBoard() const;
private:
    void initBoard(bool paused);
    void getPieceCount();
};
