#include <iostream>
#include <chrono>
#include "game.h"

Game::Game(int player, int timemin)
{
    this->player = player;
    std::cout << "Game started" << std::endl;
    board = Board(false);
    ai = Ai(player * -1);
    timer = std::make_tuple(timemin * 60 * 1000, timemin * 60 * 1000);
    gameLoop();
};
void Game::gameLoop()
{
loop:
    while (board.state == 0)
    {
        if (player == board.turn)
        {
            //std::chrono::time_point<std::chrono::system_clock> start, end;
            auto start = std::chrono::system_clock::now();
            board.printBoard();
            std::cout << "Your turn" << std::endl;
            bool turnEnd = false;
            PieceBoard pieceboard;
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
                    board.createPossibleBoards();
                    board.printBoard();
                }
                else if (notation == "moves")
                {
                    for (const auto pieceboard : board.possiblepieceboards)
                    {
                        board.printMove(pieceboard.lastmove);
                    }
                }
                else if (notation == "board")
                {
                    board.printBoard();
                }
                else if (notation == "rules")
                {
                    const char *text =
                        "----------------------------GAME RULES----------------------------\n"
                        "White vs Black player, remove all of your own pieces to win.\n\n"
                        "Both players start with 4-4 singles and doubles.\n\n"
                        "Singles can only be moved away from the owner, doubles towards.\n"
                        "Pieces can move diagonally along any number of consecutive unoccupied squares.\n"
                        "A single and a double can transpose if they are diagonally adjacent.\n\n"
                        "Doubles become singles once they get to their last row.\n"
                        "Singles become doubles once they get to their last row,\n"
                        "assuming that another single is available, which single gets removed.\n"
                        "If there isn't, they stay singles until another single becomes available.\n"
                        "If no moves are available, a single has to be removed,\n"
                        "or a double turned into a single.\n"
                        "The player with one single left and no moves available wins.\n"
                        "----------------------------GAME RULES----------------------------";
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
                        std::tie(turnEnd, pieceboard) = trySelect(pos, pieceboard);
                    }
                }
            };
            auto end = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            timer = std::make_tuple(std::get<0>(timer) - duration.count(), std::get<1>(timer));
            if (std::get<0>(timer) < 0)
            {
                std::cout << "Your time is up!" << std::endl;
                board.state = -1 * player;
            }
            else
            {
                int timeleft = std::get<0>(timer) / 1000;
                std::cout << "Time left: " << timeleft/60 << "m " << timeleft % 60 << "s" << std::endl;
                board.doMove(pieceboard);
                board.printBoard();
                board.createPossibleBoards();
            };
        }
        else
        {
            auto start = std::chrono::system_clock::now();
            std::cout << "AI turn" << std::endl;
            PieceBoard pieceboard = ai.getMove(board);
            auto end = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            timer = std::make_tuple(std::get<0>(timer), std::get<1>(timer) - duration.count());
            if (std::get<1>(timer) < 0)
            {
                std::cout << "AI's time is up!" << std::endl;
                board.state = player;
            }
            else
            {
                int timeleft = std::get<1>(timer) / 1000;
                std::cout << "Time left for AI: " << timeleft/60 << "m " << timeleft % 60 << "s" << std::endl;
                board.printMove(pieceboard.lastmove);
                board.doMove(pieceboard);
                board.createPossibleBoards();
            };
        };
    };
    if (board.state == player)
    {
        std::cout << "🎉🎉🎉 Congratulations, you won the game! 🎉🎉🎉" << std::endl;
    }
    else if (board.state == -1*player)
    {
        std::cout << "Unfortunately you lost, better luck next time!" << std::endl;
    };
};
void Game::undoMove()
{
    if (board.pieceboardhistory.size() > 1)
    {
        board.undoMove();
        board.undoMove();
        board.createPossibleBoards();
        std::cout << "Last player move undone" << std::endl;
    }
    else
    {
        std::cout << "No player moves to undo" << std::endl;
    };
};
std::tuple<bool, PieceBoard> Game::trySelect(int pos, PieceBoard pb)
{
    // look through moveset, check if there exists a move with from position
    PieceBoardVector onlymoves;
    if (pb.lastmove.from == -1)
    {
        std::copy_if(board.possiblepieceboards.begin(), board.possiblepieceboards.end(), std::inserter(onlymoves, onlymoves.end()), [&](const PieceBoard &pieceboard){ return pieceboard.lastmove.from == pos; });
        if (onlymoves.size() == 1)
        {
            return std::make_tuple(true, onlymoves.back());
        }
        else if (onlymoves.size() > 1)
        {
            pb.lastmove.from = pos;
            return std::make_tuple(false, pb);
        };
    }
    else if (pb.lastmove.to == -1)
    {
        PieceBoardVector onlymoves;
        std::copy_if(board.possiblepieceboards.begin(), board.possiblepieceboards.end(), std::inserter(onlymoves, onlymoves.end()), [&](const PieceBoard &pieceboard) { return pieceboard.lastmove.from == pb.lastmove.from && pieceboard.lastmove.to == pos; });
        if (onlymoves.size() == 1)
        {
            return std::make_tuple(true, onlymoves.back());
        }
        else if (onlymoves.size() > 1)
        {
            pb.lastmove.to = pos;
            return std::make_tuple(false, pb);
        };
    }
    else if (pb.lastmove.remove == -1)
    {
        for (PieceBoard pieceboard : board.possiblepieceboards)
        {
            if (pb.lastmove.from = pieceboard.lastmove.from && pb.lastmove.to == pieceboard.lastmove.to && pieceboard.lastmove.remove == pos)
            {
                return std::make_tuple(true, pieceboard);
            };
        };
    };
    std::cout << "No legal move available for selected position.\nEnter 'help' for help or 'moves' for valid moves" << std::endl;
    return std::make_tuple(false, PieceBoard());
};
void Game::reset()
{
    board.resetBoard(false);
};
