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
    assert(moveNumber >= 0 && moveNumber < state.possiblepieceboards.size());

    state.doMove(moveNumber);
    // state.printBoard();
    // std::cout << (state.turn == WHITE ? "white " : "black ") << " did " << state.pieceboard.lastmove.toStr() << "\n\n";
    turns += 1;
  }
  return state.pieceboard.winner;
}



int getFF(State &state, std::string &move) {
  Pos pos = Pos::parseMove(move.substr(0, 2));
  Pos to = EMPTY_POSE;
  Pos remove = EMPTY_POSE;
  if (move.size() > 2)
    to = Pos::parseMove(move.substr(2, 2));
  if (move.size() > 4)
    remove = Pos::parseMove(move.substr(4, 2));
  
  for (int i = 0; i < state.possiblepieceboards.size(); i++) {
    auto child = state.possiblepieceboards[i];
    auto cm = child.lastmove;
    if (!(cm.from == pos))
      continue;
    if (remove == EMPTY_POSE && to == EMPTY_POSE)
      return i;

    if (!(remove == EMPTY_POSE) && (to == cm.to) && (remove == cm.remove))
      return i;
    
    if (cm.to == to && remove == EMPTY_POSE)
      return i;

    if (cm.to == EMPTY_POSE && cm.remove == to)
      return i;
  }
  return -1;
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
      int ind = getFF(state, last_move);
      state.doMove(ind);
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
    int ourMove = us->decideOnBoard(state);
    std::cout << state.possiblepieceboards[ourMove].lastmove.toStr() << std::endl;
    state.doMove(ourMove);
  }
}

