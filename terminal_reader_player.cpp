#include "terminal_reader_player.h"



int TerminalReaderPlayer::decideOnBoard(State state) {
  for (int i = 0; i < ROWS; i++) {
    std::string line;
    std::cin >> line; std::cin.ignore();
    for (int j = 0; j < COLS; j++) {
      assert(state.pieceboard.getPiece(Pos(i, j)).c == line[j]);
    }
  } 
  std::string last_move;
  std::cin >> last_move; std::cin.ignore();

  int move_count;
  std::cin >> move_count; std::cin.ignore();

  assert(move_count == state.possiblepieceboards.size());
  for (int i = 0; i < move_count; i++) {
    std::string move;
    std::cin >> move; std::cin.ignore();
  }
  return player->decideOnBoard(state);
}

