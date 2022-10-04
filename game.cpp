#include <iostream>
#include <thread>
#include <chrono>
#include "game.h"

Game::Game()
{
    std::cout << "Game started" << std::endl;
    std::cout << "Choose your color (1 = White, -1 = Black): ";
    std::cin >> player;
    board = Board(false);
    ai = Ai(player * -1);
    gameLoop();
};
Game::~Game()
{
    delete boardhistory;
    std::cout << "Thanks for playing!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
};
void Game::gameLoop()
{
    while (board.state == 0)
    {
        if (player == board.turn)
        {
            board.printBoard();
            std::cout << "Your turn" << std::endl;
            bool turnEnd = false;
            Move move = Move();
            while (turnEnd == false)
            {
                std::cout << "Select position: ";
                std::string notation;
                std::cin >> notation;
                if (notation == "help")
                {
                    const char *text =
                        "Possible options:\n\n"
                        "undo: undo last move\n"
                        "moves: show possible moves\n"
                        "board: show board\n"
                        "rules: show game rules\n"
                        "restore: restore game from save\n"
                        "restart: restart game\n"
                        "quit: quit program\n";
                    std::cout << text;
                }
                else if (notation == "undo")
                {
                    undoMove();
                    board.printBoard();
                }
                else if (notation == "moves")
                {
                    for (const auto &move : board.moveset)
                    {
                        board.printMove(move);
                    }
                }
                else if (notation == "board")
                {
                    board.printBoard();
                }
                else if (notation == "rules")
                {
                    const char *text =
                        "---RULES---\n\n"
                        "White vs Black player, remove all of your own pieces to win.\n\n"
                        "Both players start with 4-4 singles and doubles.\n\n"
                        "Singles can only be moved away from the owner, doubles towards.\n"
                        "Pieces can move diagonally along any number of consecutive unoccupied squares.\n"
                        "A single and a double can transpose if they are diagonally adjacent.\n\n"
                        "Doubles become singles once they get to their last row.\n"
                        "Singles become doubles once they get to their last row,\n"
                        "assuming that another single is available, which single gets removed.\n"
                        "If there isn't, they stay single until one becomes available.\n"
                        "If no moves are available, a single has to be removed,\n"
                        "or a double turned into a single.\n"
                        "The player wins with one single left and no moves available.";
                    std::cout << text << std::endl;
                }
                else if (notation == "restart")
                {
                    std::cout << "Game restarted" << std::endl;
                    reset();
                    board.printBoard();
                }
                else if (notation == "restore")
                {
                    board.loadBoard();
                }
                else if (notation == "quit")
                {
                    board.state == -99;
                    break;
                }
                else
                {
                    int pos = parseMove(notation);
                    if (pos == -1)
                    {
                        std::cout << "Invalid position, enter 'help' for help" << std::endl;
                    }
                    else
                    {
                        int col = pos % 8;
                        int row = pos / 8;
                        std::tie(turnEnd, move) = trySelect(row, col, move);
                    }
                }
            };
            boardhistory->push_back(board);
            board.doMove(move);
        }
        else
        {
            std::cout << "AI turn" << std::endl;
            Move move = ai.getMove(board);
            board.printMove(move);
            board.doMove(move);
        };
    };
};
void Game::undoMove(){
    if (boardhistory->size() > 1){
        board = boardhistory->back();
        boardhistory->pop_back();
        std::cout << "Last player move undone" << std::endl;
    }
    else{
        std::cout << "No moves to undo" << std::endl;
    };
};
// Returns true if the selected from piece or from-to pieces make up an only single valid move, false otherwise
std::tuple<bool, bool, Move> Game::returnIfOnlyMove(const Move &move)
{
    std::set<Move> onlymoves;
    std::copy_if(board.moveset.begin(), board.moveset.end(), std::inserter(onlymoves, onlymoves.end()), [&](const Move &m) { return m.from == move.from;});
    if (onlymoves.size() == 1)
    {
        return std::make_tuple(true, true, *(onlymoves.begin()));
    }
    else if (onlymoves.size() > 1)
    {
        onlymoves.clear();
        std::copy_if(board.moveset.begin(), board.moveset.end(), std::inserter(onlymoves, onlymoves.end()), [&](const Move &m) { return std::tie(m.from, m.to) == std::tie(move.from, move.to);});
        if (onlymoves.size() == 1)
        {
            return std::make_tuple(true, true, *(onlymoves.begin()));
        }
        else
        {
            return std::make_tuple(true, false, move);
        }
    }
    else
    {
        return std::make_tuple(false, false, Move());
    };
};
std::tuple<bool, Move> Game::trySelect(int row, int col, Move move)
{
    // look through moveset, check if there exists a move with from position
    MoveSet::iterator itr;
    if (move.from.row == -1 || move.to.row == -1)
    {
        bool validmove, onlymove;
        std::tie(validmove, onlymove, move) = returnIfOnlyMove(move);
        if (validmove)
        {
            return std::make_tuple(onlymove, move);
        };
    }
    else if (move.remove.row == -1)
    {
        for (itr = board.moveset.begin(); itr != board.moveset.end(); itr++)
        {
            if (itr->from == move.from && itr->to == move.to && itr->remove.row == row && itr->remove.col == col)
            {
                return std::make_tuple(true, *itr);
            };
        };
    };
    std::cout << "Invalid move, enter 'help' for help" << std::endl;
    return std::make_tuple(false, move);
};
void Game::reset()
{
    board.resetBoard(false);
};
