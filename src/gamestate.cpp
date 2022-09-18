#include "piece.h"
#include <map>
#include <set>

typedef std::pair<int, int> Move;
typedef std::map<int, Piece> PieceMap;
typedef std::map<int, std::set<int>> MoveMap;

class GameState
{
private:
    int turn;         // 1 = white, -1 = black
    int gameState;    // 0 = in progress, 1 = white win, -1 = black win
    int whiteSingles; // number of white singles
    int whiteDoubles; // number of white doubles
    int blackSingles; // number of black singles
    int blackDoubles; // number of black doubles
    int board[8][8];
    PieceMap piecemap;
    void newGame()
    {
        turn = 1;
        gameState = 0;
        whiteSingles = 6;
        whiteDoubles = 6;
        blackSingles = 6;
        blackDoubles = 6;
        for (int col = 0; col < 8; col++)
        {
            for (int row = 0; row < 8; row++)
            {
                // 1: (0,0), (3,1), (4,0), (1,7)
                // 2: (1,7), (2,6), (5,7), (6,6)
                // -1: (0,6), (3,7), (4,6), (7,7)
                // -2: (1,1), (2,0), (5,1), (6,0)
                if (row < 2 && (col + row) % 4)
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
    int getGameState()
    {
        return gameState;
    };
    int getTurn()
    {
        return turn;
    };
    int getDirection(int color, int isDouble)
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
        int col = move.first % 8;
        int row = move.first - col;
        int newCol = move.second % 8;
        int newRow = move.second - newCol;
        board[newCol][newRow] = board[col][row];
        board[col][row] = 0;
        piecemap[move.second] = piecemap[move.first];
        piecemap.erase(col + row * 8);
    };
    void removePiece(int col, int row)
    {
        board[col][row] = 0;
        piecemap.erase(col + row * 8);
    };
    void changePieceType(int col, int row)
    {
        if (piecemap[col + row * 8].getType() == true)
        {
            piecemap[col + row * 8].setType(false);
        }
        else
        {
            piecemap[col + row * 8].setType(true);
        };
    };
    MoveMap checkDiagonals(int col, int row, bool forward)
    {
        // check diagonals forward
        int row = row;
        MoveMap movemap;
        while (row > 0 && row < 8)
        {
            int row = row + 1;
            if (forward)
            {
                for (int col = col + 1; col < 8; col++)
                {
                    if (board[col][row] == 0)
                    {
                        movemap[col + row * 8].insert(col, row);
                    }
                    else
                    {
                        break;
                    }
                }
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
        if (moves.count(move.first) && moves[move.first].count(move.second))
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
                if (piece.getColor() == color)
                {
                    int direction = getDirection(piece.getColor(), piece.getType());
                    MoveMap new_moves = checkDiagonals(col, row, direction);
                    moves.insert(new_moves.begin(), new_moves.end());
                };
            };
        };
        return moves;
    };
};