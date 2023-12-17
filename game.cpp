#include "game.h"
#include "player.h"
#include "board.h"
#include "common.h"

// Init game if savegame was available
Game::Game(Player *white, Player *black) {
  state = State();
  this->white = white;
  this->black = black;
}


int Game::gameLoop() {
  while (state.pieceboard.winner == BOARD_GAME_ONGOING) {
    int moveNumber = -1;
    if (state.turn == WHITE) {
      moveNumber = white->decideOnBoard(state);
    } else {
      moveNumber = black->decideOnBoard(state);
    }

    state.doMove(moveNumber);
    // state.printBoard();

    turns += 1;
  }
  return state.pieceboard.winner;
}

