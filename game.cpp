#include "game.h"
#include "player.h"
#include "board.h"
#include "common.h"

// Init game if savegame was available
Game::Game() {
  state = State();
}


int Game::gameLoop(Player *white, Player* black) {
  std::string lastMove = "";
  while (state.pieceboard.winner == BOARD_GAME_ONGOING) {
    int moveNumber = -1;
    if (state.turn == WHITE) {
      moveNumber = white->decideOnBoard(state, lastMove);
    } else {
      moveNumber = black->decideOnBoard(state, lastMove);
    }
    assert(moveNumber >= 0 && moveNumber < state.possiblepieceboards.size());

    state.doMove(moveNumber);
    if (print)
      state.printBoard();
    lastMove = state.pieceboard.lastmove.toStr();
    turns += 1;
  }
  return state.pieceboard.winner;
}


void Game::codingGameLoop(Player *us) {
  while (true) {
    std::string line[ROWS];
    for (int i = 0; i < ROWS; i++) {
      std::cin >> line[i]; std::cin.ignore();
    } 
    std::string last_move;
    std::cin >> last_move; std::cin.ignore();
    std::cerr << "finding the move " << last_move << std::endl;
    if (last_move != "null") {
      int ind = -1;
      for (int i = 0; i < state.possiblepieceboards.size(); i++) {
        if (state.possiblepieceboards[i].lastmove.toStr() == last_move) {
          ind = i;
          break;
        }
      }
      assert(ind != -1);
      state.doMove(ind);
    } else {
      last_move = "";
    }
    std::cerr << "found the move" << std::endl;
    for (int i = 0; i < ROWS; i++)
      for (int j = 0; j < COLS; j++)
        assert(state.pieceboard.getPiece(Pos(i, j)).c == line[i][j]);
    std::cerr << "board matches" << std::endl;

    int move_count;
    std::cin >> move_count; std::cin.ignore();

    assert(move_count == state.possiblepieceboards.size());
    std::cerr << "moves match" << std::endl;
    for (int i = 0; i < move_count; i++) {
      std::string move;
      std::cin >> move; std::cin.ignore();
    }
    int ourMove = us->decideOnBoard(state, last_move);
    std::cout << state.possiblepieceboards[ourMove].lastmove.toStr() << std::endl;
    state.doMove(ourMove);
  }
}

