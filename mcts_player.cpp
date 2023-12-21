#include "mcts_player.h"


using namespace std;

int MCTSPlayer::decideOnBoard(State state, std::string lastMove) {
  if (lastMove != "")
    mcts.updateRoot(lastMove);
  mcts.runMCTS(2000);
  int ind = mcts.getBestMove();
  mcts.ourMove(ind);
  return ind;
}

