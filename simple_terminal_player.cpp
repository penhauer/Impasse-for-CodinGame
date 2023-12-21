#include "simple_terminal_player.h"

int SimpleTerminalPlayer::decideOnBoard(State state, std::string lastMove) {
  state.printBoard();
  int movesCount = state.possiblepieceboards.size();
  int moveNumber = -1;
  while (true) {
    std::cout << "----------------------------LEGAL MOVES----------------------------\n";
    for (int i = 0; i < movesCount; i++) {
      for (int j = 0; i < movesCount && j < 4; j++, i++)
        std::cout << i + 1 << "\t" << state.possiblepieceboards[i].lastmove.toStr() << "\t\t";
      std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----------------------------YOUR TURN----------------------------" << std::endl;
    std::cout << "Select a move number from 1 to " << movesCount << std::endl;

    std::cin >> moveNumber;
    moveNumber--;
    if (moveNumber >= 0 && moveNumber < movesCount) {
      return moveNumber;
    }
  }
}

