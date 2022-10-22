#include "game.h"

// Init game if savegame was available
Game::Game()
{
    reset(true);
    gameLoop();
};
// Init new game
Game::Game(int player, int timemin)
{
    this->player = player;
    this->timemin = timemin;
    reset(false);
    gameLoop();
};
void Game::reset(bool cont)
{
    board = Board();
    if (cont)
    {
        restoreSave();
        std::cout << "Last game restored from save" << std::endl;
    }
    else
    {
        board.reset();
        timer = std::make_tuple(timemin * 60 * 1000, timemin * 60 * 1000);
    };
    std::cout << "Game started, good luck!" << std::endl;
    std::cout << "Time available: " << timemin << " minutes / player" << std::endl;
    ai = Ai(player * -1);
};
void Game::gameLoop()
{
loop:
    // While game is not over
    while (board.state == 0)
    {
        // If it's the player's turn
        if (player == board.turn)
        {
            // Keep tract of elapsed time
            auto start = std::chrono::system_clock::now();
            std::chrono::milliseconds duration;
            board.printBoard();
            std::cout << "----------------------------YOUR TURN----------------------------" << std::endl;
            bool turnEnd = false;
            PieceBoard pieceboard;
            // While player has not made a valid move, keep asking for input
            while (turnEnd == false)
            {
                std::cout << "Select position: ";
                std::string notation;
                std::cin >> notation;
                if (notation == "help")
                {
                    const char *text =
                        "----------------------------HELP----------------------------\n"
                        "howto: show how to play\n"
                        "board: show board\n"
                        "moves: show legal moves\n"
                        "time: show remaining time\n"
                        "undo: undo last move\n"
                        "restart: restart game\n"
                        "quit: quit program\n";
                    std::cout << text << std::endl;
                }
                else if (notation == "undo")
                {
                    undoPlayerMove();
                    board.generateLegalMoves();
                    board.printBoard();
                }
                else if (notation == "moves")
                {
                    std::cout << "----------------------------LEGAL MOVES----------------------------\n";
                    for (const auto pieceboard : board.possiblepieceboards)
                    {
                        printMove(pieceboard.lastmove);
                    };
                    std::cout << "\n";
                }
                else if (notation == "board")
                {
                    board.printBoard();
                    std::cout << "\n";
                }
                else if (notation == "time")
                {
                    std::cout << "----------------------------REMAINING TIME----------------------------\n";
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
                    int playertime = (std::get<0>(timer) - duration.count()) / 1000;
                    int aitime = std::get<1>(timer) / 1000;
                    std::cout << "Player: " << playertime / 60 << "m " << playertime % 60 << "s\nAI: " << aitime / 60 << "m " << aitime % 60 << "s\n"
                              << std::endl;
                }
                else if (notation == "howto")
                {
                    const char *text =
                        "----------------------------HOW TO PLAY----------------------------\n"
                        "White vs Black player, remove all of your own pieces to win.\n\n"
                        "Both players start with 4-4 singles and doubles.\n\n"
                        "Singles can only be moved away from the owner, doubles towards.\n"
                        "Pieces can move diagonally along any number of consecutive unoccupied squares.\n"
                        "A single and a double can transpose if they are adjacent (diagonally).\n\n"
                        "Doubles become singles once they get to their last row.\n"
                        "Singles become doubles once they get to their last row,\n"
                        "assuming that another single is available, which single gets removed.\n"
                        "If there isn't, they stay singles until another single becomes available.\n"
                        "If no moves are available, a single has to be removed,\n"
                        "or a double turned into a single.\n"
                        "The player with a lone single left and no moves available wins.\n\n"
                        "How to move:\n"
                        "Select the position of the piece you want to move (e.g. input D2 and press enter),\n"
                        "then select the position you want to move it to. If needed, select piece to remove as well.\n";
                    std::cout << text << std::endl;
                }
                else if (notation == "restart")
                {
                    std::cout << "Do you want to switch sides? (yes/no): ";
                    std::string answer;
                    std::cin >> answer;
                    if (answer == "yes")
                    {
                        player *= -1;
                        reset(false);
                    }
                    else
                    {
                        reset(false);
                    };
                    std::cout << "Game restarted!" << std::endl;
                    goto loop;
                }
                else if (notation == "quit")
                {
                    board.state = -99;
                    goto loop;
                }
                else
                {
                    // Else, try to parse input into a move coordinate
                    int pos = parseMove(notation);
                    // If not valid position, ask for input again
                    if (pos == -1)
                    {
                        std::cout << "Invalid position, enter 'help' for help" << std::endl;
                    }
                    // Else try to make a move based on the given inputs
                    else
                    {
                        std::tie(turnEnd, pieceboard) = trySelect(pos, pieceboard);
                    }
                };
                duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
                if (std::get<0>(timer) - duration.count() < 0)
                {
                    std::cout << "Your time is up!" << std::endl;
                    board.state = -1 * player;
                    goto loop;
                };
            };
            std::get<0>(timer) -= duration.count();
            int timeleft = std::get<0>(timer) / 1000;
            std::cout << "Time left: " << timeleft / 60 << "m " << timeleft % 60 << "s" << std::endl;
            board.doMove(pieceboard);
            board.printBoard();
            board.generateLegalMoves();
            save();
        }
        else
        {
            auto start = std::chrono::system_clock::now();
            std::cout << "AI turn" << std::endl;
            PieceBoard pieceboard = ai.getMove(board, std::get<1>(timer));
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
            if (std::get<1>(timer) - duration.count() < 0)
            {
                std::cout << "AI's time is up!" << std::endl;
                board.state = player;
            }
            else
            {
                std::get<1>(timer) -= duration.count();
                int timeleft = std::get<1>(timer) / 1000;
                std::cout << "Time left for AI: " << timeleft / 60 << "m " << timeleft % 60 << "s" << std::endl;
                printMove(pieceboard.lastmove);
                board.doMove(pieceboard);
                board.generateLegalMoves();
                save();
            };
        };
    };
    if (board.state == player)
    {
        std::cout << "🎉🎉🎉 Congratulations, you won the game! 🎉🎉🎉" << std::endl;
        deleteSave();
    }
    else if (board.state == -1 * player)
    {
        std::cout << "Unfortunately you lost, better luck next time!" << std::endl;
        deleteSave();
    }
};
// Check if a move exists among the current legal moves with the selected position
// If only one move exists, make that move, else ask for the next position
// If no move exists with the selected position, reset move and ask for input again
std::tuple<bool, PieceBoard> Game::trySelect(int pos, PieceBoard pb)
{
    PieceBoardVector onlymoves;
    if (pb.lastmove.from == -1)
    {
        std::copy_if(board.possiblepieceboards.begin(), board.possiblepieceboards.end(), std::inserter(onlymoves, onlymoves.end()), [&](const PieceBoard &pieceboard)
                     { return pieceboard.lastmove.from == pos; });
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
        std::copy_if(board.possiblepieceboards.begin(), board.possiblepieceboards.end(), std::inserter(onlymoves, onlymoves.end()), [&](const PieceBoard &pieceboard)
                     { return pieceboard.lastmove.from == pb.lastmove.from && pieceboard.lastmove.to == pos; });
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
// Undo the last AI and player move if possible
void Game::undoPlayerMove()
{
    if (board.pieceboardhistory.size() > 1)
    {
        board.undoMove();
        board.undoMove();
        board.generateLegalMoves();
        std::cout << "Last player move undone" << std::endl;
    }
    else
    {
        std::cout << "No player moves to undo" << std::endl;
    };
};
// Save the current game state to a file
void Game::save() const
{
    std::ofstream newfile;
    newfile.open(".savegame", std::ios::out);
    if (newfile.is_open())
    {
        newfile << player << std::endl;
        newfile << timemin << std::endl;
        newfile << std::get<0>(timer) << std::endl;
        newfile << std::get<1>(timer) << std::endl;
        newfile << board.turn << std::endl;
        newfile << board.state << std::endl;
        newfile << board.pieceboard.postocrown.size() << std::endl;
        for (auto x : board.pieceboard.postocrown)
        {
            newfile << x.first << " " << x.second << std::endl;
        };
        newfile << board.pieceboard.piecemap.size() << std::endl;
        for (auto x : board.pieceboard.piecemap)
        {
            newfile << x.first << " " << x.second.piece << " " << x.second.color << std::endl;
        };
        newfile.close();
    };
    newfile.close();
};
// Restore the game state from a file
void Game::restoreSave()
{
    // if .savegame exist, load variables
    std::ifstream newfile;
    newfile.open(".savegame", std::ios::in);
    if (newfile.is_open())
    {
        Board savedboard;
        newfile >> player;
        newfile >> timemin;
        newfile >> std::get<0>(timer);
        newfile >> std::get<1>(timer);
        newfile >> savedboard.turn;
        newfile >> savedboard.state;
        int postocrownsize;
        newfile >> postocrownsize;
        for (int i = 0; i < postocrownsize; i++)
        {
            int color;
            int pos;
            newfile >> color;
            newfile >> pos;
            savedboard.pieceboard.postocrown[color] = pos;
        };
        int piecemapsize;
        newfile >> piecemapsize;
        for (int i = 0; i < piecemapsize; i++)
        {
            int pos;
            int piece;
            int color;
            newfile >> pos;
            newfile >> piece;
            newfile >> color;
            savedboard.pieceboard.piecemap[pos] = Piece(piece, color, pos);
        };
        newfile.close();
        board.reset(savedboard);
        deleteSave();
    };
};
// If saved board exists, delete it
void Game::deleteSave() const
{
    std::ifstream newfile;
    newfile.open(".savegame", std::ios::in);
    if (newfile.is_open())
    {
        newfile.close();
        std::remove(".savegame");
    };
};
