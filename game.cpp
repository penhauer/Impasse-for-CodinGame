#include <string>
#include <tuple>
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
    std::cout << "Thanks for playing!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    //board.deleteBoard();
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
            Move move;
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
                    "restart: restart game\n"
                    "quit: quit program\n";
                    std::cout << text;
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
                else if (notation == "rules")
                {
                    const char *text =
                    "---RULES---\n\n"
                    "Object: First to remove all of their own pieces wins.\n\n"
                    "Each player starts with 4-4 single and double pieces.\n\n"
                    "Singles can be moved away from the owner, while doubles towards.\n"
                    "Pieces move diagonally along any number of consecutive unoccupied squares.\n"
                    "A single and a double can transpose if they are adjacent diagonally.\n\n"
                    "Doubles become singles once they get to their last row.\n"
                    "Singles turn into doubles once they get to their last row\n"
                    "if another single piece is available, and the single gets removed.\n"
                    "If there are no other singles available, they stay singles until there are,\n"
                    "after which they get turned into doubles.\n\n"
                    "If there are no moves available, the player has to remove a single\n"
                    "or turn a double into a single.\n"
                    "The game ends with a lone single with no moves available.";
                    std::cout << text << std::endl;
                }
                else if (notation == "restart")
                {
                    reset();
                }
                else if (notation == "quit")
                {
                    turnEnd == true;
                    board.state == -99;
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
    };
};
std::tuple<bool, Move> Game::trySelect(int pos, Move move)
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
    std::cout << "Invalid move, enter 'help' for help" << std::endl;
    return std::make_tuple(false, move);
};
void Game::reset()
{
    board.resetBoard(false);
};
//concert from chess notation to position
int Game::parseMove(const std::string& notation)
{
    if (notation.size() != 2)
    {
        return -1;
    };
    char letter = tolower(notation[0]);
    int row = notation[1] - '1';
    int col = (int)letter - 97;
    row = (int)row;
    int pos = col + 8 * row;
    if (pos < 0 || pos > 63)
    {
        return -1;
    };
    return pos;
};
