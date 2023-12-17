#include "game.h"
#include "player.h"
#include "board.h"
#include "common.h"

// Init game if savegame was available
Game::Game() {
  board = Board();
}


int Game::gameLoop(Player *white, Player *black) {
  // While game is not over
  while (board.winner == BOARD_GAME_ONGOING) {
    // If it's the player's turn
    int moveNumber = -1;
    if (board.turn == WHITE) {
      // std::cout << "white's turn: " << std::endl;
      moveNumber = white->decideOnBoard(board);
    } else {
      // std::cout << "black's turn: " << std::endl;

      // std::cout << "\n";
      // auto start = std::chrono::system_clock::now();
      // std::cout << "AI turn" << std::endl;
      // PieceBoard pieceboard = ai.getMove(board);
      // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
      // if (std::get<1>(timer) - duration.count() < 0)
      // {
      //     std::cout << "AI's time is up!" << std::endl;
      //     board.winner = WHITE;
      // } else {
      //     std::get<1>(timer) -= duration.count();
      //     int timeleft = std::get<1>(timer) / 1000;
      //     std::cout << "Time left for AI: " << timeleft / 60 << "m " << timeleft % 60 << "s" << std::endl;
      //     std::cout << board.possiblepieceboards[moveNumber].lastmove.toStr() << std::endl;
      // }

      moveNumber = black->decideOnBoard(board);
    }

    board.doMove(moveNumber);

    turns += 1;
  }
  return board.winner;
}

