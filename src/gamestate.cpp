#include <map>
#include <unordered_map>
#include <set>
#include <iostream>

// Map of the board represented by integers
typedef std::map<int, std::map<int, int>> Board;

// Simple struct to represent a position on the board
struct Pos {
    int col;
    int row;
    Pos(int col, int row) : col(col), row(row) {};
};
// Simple struct to represent a move
struct Move
{
    Pos from;
    Pos to;
    Pos remove;
    Move(Pos from, Pos to, Pos remove = Pos(-1, -1)) : from(from), to(to), remove(remove) {};
};

// Map of all possible moves at the moment
typedef std::set<Pos> MoveSet;
typedef std::unordered_map<Pos, MoveSet> MoveMap;

struct MoveSetPair {
    Pos pos;
    MoveSet moves;
    MoveSetPair(Pos pos, MoveSet moves) : pos(pos), moves(moves) {};
};
struct Pieces
{
    int whiteSingles; // number of white singles
    int whiteDoubles; // number of white doubles
    int blackSingles; // number of black singles
    int blackDoubles; // number of black doubles
    Pieces() : whiteSingles(6), whiteDoubles(6), blackSingles(6), blackDoubles(6){};
};

class GameState
{
public:
    int player; // 0 = Ai vs Ai, 1 = White vs Ai, -1 = Black vs Ai, 2 = White vs Black
    int turn;   // 1 = white, -1 = black
    int state;  // 0 = in progress, 1 = white win, -1 = black win
    Pieces pieces;
    Board board;
    MoveMap movemap;
    GameState()
    {
        player = 1;
        turn = 1;
        state = 0;
        pieces = Pieces();
        for (int col = 0; col < 8; col++)
        {
            for (int row = 0; row < 8; row++)
            {
                // white single (1): (0,0), (3,1), (4,0), (1,7)
                // white double (2): (1,7), (2,6), (5,7), (6,6)
                // black single (-1): (0,6), (3,7), (4,6), (7,7)
                // black double (-2): (1,1), (2,0), (5,1), (6,0)
                if (row < 2 && (col + row) % 4 == 0)
                {
                    board[col][row] = 1;
                }
                else if (row > 5 && (col + row) % 4 == 0)
                {
                    board[col][row] = 2;
                }
                else if ((row > 5) && ((col + row) % 4 == 2))
                {
                    board[col][row] = -1;
                }
                else if (row < 2 && (col + row) % 4 == 2)
                {
                    board[col][row] = -2;
                }
                else
                {
                    board[col][row] = 0;
                };
            };
        };
    };
    int pieceDirection(const int &piece) const
    {
        return (piece == 1 || piece == -2) ? 1 : -1;
    };
    void movePiece(const Move &move)
    {
        board[move.to.col][move.to.row] = board[move.from.col][move.from.row];
        board[move.from.col][move.from.row] = 0;
    };
    void removePiece(const Pos& pos) {
        int& piece = board[pos.col][pos.row];
        switch (piece) {
            case 1:
                pieces.whiteSingles--;
                break;
            case 2:
                pieces.whiteDoubles--;
                break;
            case -1:
                pieces.blackSingles--;
                break;
            case -2:
                pieces.blackDoubles--;
                break;
        };
        piece = 0;
        // chickendinner
        if (pieces.whiteSingles == 0 && pieces.whiteDoubles == 0) {
            state = -1;
        }
        else if (pieces.blackSingles == 0 && pieces.blackDoubles == 0) {
            state = 1;
        }
    };
    void changePieceType(const Move& move)
    {
        int& piece = board[move.from.col][move.from.row];
        switch (piece) {
        case 1:
            piece = 2;
            break;
        case 2:
            piece = 1;
            break;
        case -1:
            piece = -2;
            break;
        case -2:
            piece = -1;
            break;
        };
    };
    MoveSetPair checkPieceDiagonals(const int &col, const int &row)
    {
        // check diagonals forward
        const int &piece = board[col][row];
        const int &direction = pieceDirection(piece);
        MoveSet moves;
        // right
        for (int c = col + 1; c < 8; c++)
        {
            int r = row + direction * (c - col);
            if (r < 0 || r > 7)
                break;
            const int &square = board[c][r];
            // transpose
            if (c - col == 1 && (square + piece) % 3 == 0)
            {
                moves.insert(Pos(c, r));
                break;
            }
            // slide
            else if (square == 0)
            {
                moves.insert(Pos(c, r));
            }
            else
            {
                break;
            }
        };
        // left
        for (int c = col - 1; c >= 0; c--)
        {
            int r = row + direction * (col - c);
            if (r < 0 || r > 7)
                break;
            const int &square = board[c][r];
            // transpose
            if (col - c == 1 && (square + piece) % 3 == 0)
            {
                moves.insert(Pos(c, r));
                break;
                // slide
            }
            else if (square == 0)
            {
                moves.insert(Pos(c, r));
            }
            else
            {
                break;
            }
        };
        MoveSetPair movesetpair(Pos(col, row), moves);// = MoveSetPair(Pos(col, row), MoveSet());
        return movesetpair;
    };
    void makeMoveMap()
    {
        movemap.clear();
        // return array of possible moves
        for (int col = 0; col < 8; col++)
        {
            for (int row = 0; row < 8; row++)
            {
                const int &piece = board[col][row];
                if (piece * turn > 0) // if same color
                {
                    MoveSetPair movesetpair = checkPieceDiagonals(col, row);
                    if (movesetpair.moves.size() > 0)
                    {
                        movemap[movesetpair.pos] = movesetpair.moves;
                    }
                };
            };
        };
        if (movemap.size() == 0)
        {
            MoveSetPair deleteable = checkDeleteable();
            if (deleteable.moves.size() > 0)
            {
                movemap[deleteable.pos] = deleteable.moves;
            }
        };
    };
    MoveSetPair checkDeleteable() {
        for (int col = 0; col < 8; col++)
        {
            for (int row = 0; row < 8; row++)
            {
                const int &piece = board[col][row];
                if ((piece * turn > 0) && /*in last row*/ )
                {
                    MoveSet moves;
                    moves.insert(Pos(col, row));
                    MoveSetPair movesetpair = MoveSetPair(Pos(col, row), moves);
                    return movesetpair;
                };
            };
        };
    };
    void makeMove(const Move& move)
    {
        // if king / bearoff
        if (move.remove.)
        {
            changePieceType(move);
        }
        else if (pieceDirection(move.piece) == -1 && move.to.row == 0)
        {
            changePieceType(move);
        }
        else
        {
            movePiece(move);
        };
        // if move is delete piece ...
        // if move is king piece ...
        movePiece(move);
        turn = turn * -1; //because of the way the removal is implemented (after main move, and we want to visualize it)
        // one way to handle it is remove turn = turn * -1 from here, and handle the turns in main.cpp.
        // However then the changePieceType has to be changed, as Move shouldn't include the remove piece, instead only once the piece moves to the last row,
        // should there be a mandatory next move, to crown / bearoff the piece, using changepiecetype
        makeMoveMap();
    };
};
int main()
{
    GameState gamestate = GameState();
    gamestate.makeMoveMap();
    return 0;
}