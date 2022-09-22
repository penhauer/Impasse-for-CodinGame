#include <map>
#include <unordered_set>
#include <tuple>

// Simple struct to represent a move
struct Move {
    int piece;
    std::pair<int,int> from;
    std::pair<int,int> to;
};

// Map of the board represented by integers
typedef std::map<int, std::map<int, int>> Board;

// Map of all possible moves at the moment
typedef std::map<std::pair<int,int>, std::unordered_set<int>> MoveMap;

struct Pieces {
    int whiteSingles; // number of white singles
    int whiteDoubles; // number of white doubles
    int blackSingles; // number of black singles
    int blackDoubles; // number of black doubles
    Pieces () : whiteSingles(6), whiteDoubles(6), blackSingles(6), blackDoubles(6) {};
};

class GameState
{
public:
    int player; // 0 = Ai vs Ai, 1 = White vs Ai, -1 = Black vs Ai, 2 = White vs Black
    int turn;         // 1 = white, -1 = black
    int state;    // 0 = in progress, 1 = white win, -1 = black win
    Pieces pieces;
    Board board;
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
    int pieceDirection(int piece) const
    {
        return (piece == 1 || piece == -2) ? 1 : -1;
    };
    void movePiece(const Move& move)
    {
        board[move.to.first][move.to.second] = board[move.from.first][move.from.second];
        board[move.from.first][move.from.second] = 0;
    };
    void changePieceType(int col, int row) {
        board[col][row] = board[col][row] % 2 == 0 ? board[col][row] / 2 : board[col][row] * 2;
    };
    MoveMap checkDiagonals(int col, int row) const
    {
        // check diagonals forward
        MoveMap movemap;
        while (row < 8)
        {
                for (col < 8; col++)
                {
                    int piece = board[col][row];
                    if (pieceDirection() == 1) {
                    if (board.at(col).at(row) == 0)
                    {
                        movemap[getPosition(col, row)].insert(col, row);
                    }
                    else
                    {
                        break;
                    }
                };
                // code here
                row = row + 1;
            }
            else
            {
                // code here
                row = row - 1;
            };
        };
        return movemap;
    };
    void makeMove(Move move)
    {
        MoveMap moves = getMoves(turn);
        // TODO hashmap instead of set for MoveMap with type of move and maybe already the reward
        if (moves.count(move.from) && moves[move.from].count(move.to))
        {
            // if move is delete piece ...
            // if move is king piece ...
            // if move is impasse ...
            movePiece(move);
        };
        turn = turn * -1;
    };
    MoveMap getMoves(int color)
    {
        // return array of possible moves
        MoveMap moves;
        for (int col = 0; col < 8; col++)
        {
            for (int row = 0; row < 8; row++)
            {
                Piece piece = piecemap[getPosition(col, row)];
                if (piece.color == color)
                {
                    int direction = pieceDirection(piece.color, piece.isDouble);
                    MoveMap new_moves = checkDiagonals(col, row, direction);
                    moves.insert(new_moves.begin(), new_moves.end());
                };
            };
        };
        return moves;
    };
};