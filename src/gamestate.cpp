#include <unordered_map>
#include <unordered_set>

struct Piece
{
    int color;     // 1 = white, -1 = black
    bool isDouble; // true = double, false = single
    Piece (int color, bool isDouble) : color(color), isDouble(isDouble) {}
};

// Simple struct to represent a move
struct Move {
    int from;
    int to;
};

// Map of pieces on the board
typedef std::unordered_map<int, Piece> PieceMap;
// Map of all possible moves at the moment
typedef std::unordered_map<int, std::unordered_set<int>> MoveMap;

// Map of the board represented by integers
typedef std::unordered_map<int, std::unordered_map<int, int>> Board;

class GameState
{
private:
    int player; // 0 = Ai vs Ai, 1 = White vs Ai, -1 = Black vs Ai, 2 = White vs Black
    int turn;         // 1 = white, -1 = black
    int gameState;    // 0 = in progress, 1 = white win, -1 = black win
    int whiteSingles; // number of white singles
    int whiteDoubles; // number of white doubles
    int blackSingles; // number of black singles
    int blackDoubles; // number of black doubles
    Board board;

    PieceMap piecemap;
    void newGame()
    {
        player = 1;
        turn = 1;
        gameState = 0;
        whiteSingles = 6;
        whiteDoubles = 6;
        blackSingles = 6;
        blackDoubles = 6;
        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                // 1: (0,0), (3,1), (4,0), (1,7)
                // 2: (1,7), (2,6), (5,7), (6,6)
                // -1: (0,6), (3,7), (4,6), (7,7)
                // -2: (1,1), (2,0), (5,1), (6,0)
                if (row < 2 && (col + row) % 4 == 0)
                {
                    board[col][row] = 1;
                    piecemap[col + row * 8] = Piece(1, false);
                }
                else if (row > 5 && (col + row) % 4 == 0)
                {
                    board[col][row] = 2;
                    piecemap[col + row * 8] = Piece(1, true);
                }
                else if ((row > 5) && ((col + row) % 4 == 2))
                {
                    board[col][row] = -1;
                    piecemap[col + row * 8] = Piece(-1, false);
                }
                else if (row < 2 && (col + row) % 4 == 2)
                {
                    board[col][row] = -2;
                    piecemap[col + row * 8] = Piece(-1, true);
                }
                else
                {
                    board[col][row] = 0;
                };
            };
        };
    };

public:
    GameState()
    {
        newGame();
    };
    // getters
    int getGameState() const
    {
        return gameState;
    };
    int getTurn() const
    {
        return turn;
    };
    int getDirection(int color, int isDouble) const
    {
        if ((color == 1 && !isDouble) | (color == -1 && isDouble))
        {
            return 1;
        }
        else
        {
            return -1;
        };
    };
    // setters
    void movePiece(Move move)
    {
        int col = move.from % 8;
        int row = move.from - col;
        int newCol = move.to % 8;
        int newRow = move.to - newCol;
        board[newCol][newRow] = board[col][row];
        board[col][row] = 0;
        piecemap[move.to] = piecemap[move.from];
        piecemap.erase(col + row * 8);
    };
    void removePiece(int col, int row)
    {
        board[col][row] = 0;
        piecemap.erase(col + row * 8);
    };
    void changePieceType(int col, int row)
    {
        if (piecemap[col + row * 8].isDouble == true)
        {
            piecemap[col + row * 8].isDouble = false;
        }
        else
        {
            piecemap[col + row * 8].isDouble = true;
        };
    };
    MoveMap checkDiagonals(int col, int row, bool forward) const
    {
        // check diagonals forward
        MoveMap movemap;
        while (row > 0 && row < 8)
        {
            if (forward)
            {
                for (int col = col + 1; col < 8; col++)
                {
                    if (board.at(col).at(row) == 0)
                    {
                        movemap[col + row * 8].insert(col, row);
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
                Piece piece = piecemap[col + row * 8];
                if (piece.color == color)
                {
                    int direction = getDirection(piece.color, piece.isDouble);
                    MoveMap new_moves = checkDiagonals(col, row, direction);
                    moves.insert(new_moves.begin(), new_moves.end());
                };
            };
        };
        return moves;
    };
};