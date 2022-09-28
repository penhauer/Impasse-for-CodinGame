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
    Game(int gui, int player)
    {
        board = Board();
        ai = Ai(player*-1);
        gameLoop();
    };
private:
    void gameLoop()
    {
        Move move;
        while (board.state == 0)
        {
            if (player == board.turn)
            {
                std::cout << "Your turn" << std::endl;
                bool turnEnd = false;
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
                        board = Board();
                    }
                    else if (notation == "quit")
                    {
                        std::cout << "Thanks for playing!" << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                        board.deleteBoard();
                        exit(0);
                    }
                    else
                    {
                        int pos = parseMove(notation);
                        std::tie(turnEnd, move) = trySelect(pos);
                    }
                };
                board.doMove(move);
            }
            else
            {
                std::cout << "AI turn" << std::endl;
                //create timer
                Move move = ai.getMove(board);
                board.doMove(move);
            };
            board.printBoard();
        };
    };
    std::tuple<bool, Move> trySelect(int pos)
    {
        if (board.movemap.count(pos) == 0)
        {
            std::cout << "Invalid position" << std::endl;
            return false, Move;
        }
        else
        {
            const MoveSet &moveset = board.movemap[pos];
            if (moveset.size() == 1)
            {
                Move move = Move(pos, moveset.begin);
                return true;
            }
            else
            {
                std::cout << "Select move: ";
                std::string notation;
                std::cin >> notation;
                int move = parseMove(notation);
                if (moveset.count(move) == 0)
                {
                    std::cout << "Invalid move" << std::endl;
                    return false;
                }
                else
                {
                    move = Move(pos, move);
                    return true;
                };
            };
        }
        if (
        {
            
            return true;
        }
        else
        {
            return false;
        };
    };
    void reset()
    {
        board.resetBoard();
    };
    //concert from chess notation to position
    int parseMove(const std::string &notation)
    {
        char letter = tolower(notation[0]);
        int row = notation[1] - '1';
        int col = (int)letter-97;
        row = (int)row;
        return col + 8 * row;
    };
};