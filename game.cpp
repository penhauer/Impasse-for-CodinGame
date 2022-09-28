#include <string>
#include <tuple>
#include <iostream>
#include <thread>
#include <chrono>
#include "game.h"

class Game
{
private:
    int player; // 1 = White vs Ai, -1 = Black vs Ai
    int gui;    // 0 = no gui, 1 = gui
    Board board;
    Ai ai;
public:
    Game()
    {
        std::cout << "Game started" << std::endl;
        std::cout << "Choose your color (1 = White, -1 = Black): ";
        std::cin >> player;
        Board board = Board(false);
        Ai ai = Ai(player*-1);
        gameLoop();
    };
    ~Game()
    {
        std::cout << "Thanks for playing!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        //board.deleteBoard();
    };
private:
    void gameLoop()
    {
        while (board.state == 0)
        {
            if (player == board.turn)
            {
                std::cout << "Your turn" << std::endl;
                bool turnEnd = false;
                Move move;
                while (turnEnd)
                {
                    std::cout << "Select position, write help for help: ";
                    std::string notation;
                    std::cin >> notation;
                    if (notation == "help")
                    {
                        std::cout << "Possible options:\nundo: undo last move\nmoves: print possible moves\nboard: print board\nrestart: restart game\nquit: quit program\n";
                    }
                    else if (notation == "undo")
                    {
                        board.undoMove();
                    }
                    else if (notation == "moves")
                    {
                        board.printMoves();
                    }
                    else if (notation == "board")
                    {
                        board.printBoard();
                    }
                    else if (notation == "restart")
                    {
                        reset();
                    }
                    else if (notation == "quit")
                    {
                        exit(0);
                    }
                    else
                    {
                        int pos = parseMove(notation);
                        if (pos == -1)
                        {
                            std::cout << "Invalid position" << std::endl;
                        }
                        else
                        {
                            std::tie(turnEnd, move) = trySelect(pos, move);
                        }
                    }
                };
                board.doMove(move);
            }
            else
            {
                std::cout << "AI turn" << std::endl;
                Move move = ai.getMove(board);
                board.doMove(move);
            };
            board.printBoard();
        };
    };
    std::tuple<bool, Move> trySelect(int pos, Move move)
    {
        //look through moveset, check if there exists a move with from position
        MoveSet::iterator itr;
        for (itr = board.moveset.begin(); itr != board.moveset.end(); itr++)
        { // TODO implement
            if (itr->from.pos == pos)
            {
                move.from = Piece();
                return std::make_tuple(false, move);
            }
            else
            {
                int piece = board.boardarray[pos];
                move.from = Piece(0, pos);
                return std::make_tuple(false, move);
            };
        };
        std::cout << "Invalid move" << std::endl;
        return std::make_tuple(false, move);
    };
    void reset()
    {
        board.resetBoard(false);
    };
    //concert from chess notation to position
    int parseMove(const std::string &notation)
    {
        if (notation.size() != 2)
        {
            return -1;
        };
        char letter = tolower(notation[0]);
        int row = notation[1] - '1';
        int col = (int)letter-97;
        row = (int)row;
        return col + 8 * row;
    };
};