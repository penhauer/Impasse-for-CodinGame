#include "game.h"
#include "player.h"
#include "board.h"
#include "common.h"

// Init game if savegame was available
Game::Game(int timemin)
{
    this->timemin = timemin;
    board = Board();
    timer = std::make_tuple(timemin * 60 * 1000, timemin * 60 * 1000);
    std::cout << "Game started, good luck!" << std::endl;
    std::cout << "Time available: " << timemin << " minutes / player" << std::endl;
    ai = Ai(WHITE * -1);
}


int Game::getMoveFromPlayer() {
  return 0;
}

int Game::gameLoop(Player *white, Player *black) {
    // While game is not over
    while (board.winner == BOARD_GAME_ONGOING) {
        // If it's the player's turn
        if (board.turn == WHITE) {
          white->decideOnBoard(board);
        } else {
          std::cout << "\n";
            auto start = std::chrono::system_clock::now();
            std::cout << "AI turn" << std::endl;
            PieceBoard pieceboard = ai.getMove(board, std::get<1>(timer));
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
            if (std::get<1>(timer) - duration.count() < 0)
            {
                std::cout << "AI's time is up!" << std::endl;
                board.winner = WHITE;
            }
            else
            {
                std::get<1>(timer) -= duration.count();
                int timeleft = std::get<1>(timer) / 1000;
                std::cout << "Time left for AI: " << timeleft / 60 << "m " << timeleft % 60 << "s" << std::endl;
                printMove(pieceboard.lastmove);
                board.doMove(pieceboard);
            }
        }
    }
    return board.winner;
}

