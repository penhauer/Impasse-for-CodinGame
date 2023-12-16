#include "simple_terminal_player.h"
#include "common.h"

void SimpleTerminalPlayer::decideOnBoard(Board &board) {
  board.printBoard();
  std::cout << "----------------------------YOUR TURN----------------------------" << std::endl;
  int movesCount = board.possiblepieceboards.size();
  int moveNumber;
  while (true) {
    std::cout << "----------------------------LEGAL MOVES----------------------------\n";
    for (int i = 0; i < movesCount; i++) {
      std::cout << i + 1 << "\t" << board.possiblepieceboards[i].lastmove.toStr() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Select a move number from 1 to " << movesCount << std::endl;

    std::cin >> moveNumber;
    moveNumber--;
    if (moveNumber >= 0 && moveNumber < movesCount) {
      break;
    }
  }
  board.doMove(board.possiblepieceboards[moveNumber]);
  board.printBoard();
}

