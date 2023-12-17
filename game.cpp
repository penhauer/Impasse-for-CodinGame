#include "game.h"
#include "player.h"
#include "board.h"
#include "common.h"

// Init game if savegame was available
Game::Game(Player *white, Player *black) {
  board = State();
  this->white = white;
  this->black = black;
}


int Game::gameLoop() {
  while (board.winner == BOARD_GAME_ONGOING) {
    int moveNumber = -1;
    if (board.turn == WHITE) {
      moveNumber = white->decideOnBoard(board);
    } else {
      moveNumber = black->decideOnBoard(board);
    }

    board.doMove(moveNumber);
    // board.printBoard();

    turns += 1;
  }
  return board.winner;
}

