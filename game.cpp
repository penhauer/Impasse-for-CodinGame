#include <iostream>
#include "game.h"

Game::Game(int player)
{
    this->player = player;
    std::cout << "Game started" << std::endl;
    board = Board(false);
    ai = Ai(player * -1);
    gameLoop();
};
Game::~Game()
{
    if (board.state == 1)
    {
        
    }
    delete boardhistory;
};
void Game::gameLoop()
{
loop:
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
                    /*for (const auto &move : board.moveset)
                    {
                        board.printMove(move);
                    }*/
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
                    //board.loadBoard();
                }
                else if (notation == "quit")
                {
                    board.state = -99;
                    goto loop;
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
                        std::tie(turnEnd, pieceboard) = trySelect(row, col, move);
                    }
                }
            };
            boardhistory->push_back(board);
            board.doMove(pieceboard);
        }
        else
        {
            std::cout << "AI turn" << std::endl;
            Move move = ai.getMove(board);
            board.printMove(move);
            board.doMove(move);
        };
    };
    if (board.state == player)
    {
        std::cout << "🎉🎉🎉 Congratulations, you won the game! 🎉🎉🎉" << std::endl;
    }
    else if (board.state == -1)
    {
        std::cout << "Unfortunately you lost, better luck next time!" << std::endl;
    };
};
void Game::undoMove()
{
    if (boardhistory->size() > 0)
    {
        board = boardhistory->back();
        boardhistory->pop_back();
        std::cout << "Last player move undone" << std::endl;
    }
    else
    {
        std::cout << "No moves to undo" << std::endl;
    };
};
std::tuple<bool, Move> Game::returnIfOnlyMove(const Move &move)
{
    std::set<Move> onlymoves_from;
    std::copy_if(board.possibleboards.begin(), board.possibleboards.end(), std::inserter(onlymoves_from, onlymoves_from.end()), [&](const Move &m){ return m.from == move.from; });
    if (onlymoves_from.size() == 1)
    {
        return std::make_tuple(true, *(onlymoves_from.begin()));
    }
    else if (onlymoves_from.size() > 1)
    {
        std::set<Move> onlymoves_to;
        std::copy_if(onlymoves_from.begin(), onlymoves_from.end(), std::inserter(onlymoves_to, onlymoves_to.end()), [&](const Move &m) { return m.to == move.to; });
        if (onlymoves_to.size() == 1)
        {
            return std::make_tuple(true, *(onlymoves_to.begin()));
        };
    };
    return std::make_tuple(false, move);
};
std::tuple<bool, Move> Game::trySelect(int row, int col, Move move)
{
    // look through moveset, check if there exists a move with from position
    MoveSet::iterator itr;
    if (move.from.row == -1)
    {
        for (itr = board.moveset.begin(); itr != board.moveset.end(); itr++)
        {
            if (itr->from.row == row && itr->from.col == col)
            {
                move.from = itr->from;
                return returnIfOnlyMove(move);
            };
        };
    }
    else if (move.to.row == -1)
    {
        for (itr = board.moveset.begin(); itr != board.moveset.end(); itr++)
        {
            if (itr->from == move.from && itr->to.row == row && itr->to.col == col)
            {
                move.to = itr->to;
                return returnIfOnlyMove(move);
            };
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
    std::cout << "No legal move available for selected position.\nEnter 'help' for help or 'moves' for valid moves" << std::endl;
    return std::make_tuple(false, Move());
};
void Game::reset()
{
    board.resetBoard(false);
};
