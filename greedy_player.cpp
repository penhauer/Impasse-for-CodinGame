#include "greedy_player.h"
#include "board.h"


int GreedyPlayer::evaluateBoard(PieceBoard &pieceBoard) {
  int c = color == WHITE ? 1 : -1;
  int colorC[2] = {-c, c};
  PieceCount *pc = &pieceBoard.piececount;
  int pieceValue = colorC[1] * (2 * pc->blackSingles + 3 * pc->blackDoubles) + colorC[0] * (2 * pc->whiteSingles - 3 * pc->whiteDoubles);

  int distanceValue[2] = {0, 0};
  for (int i = 0; i < ROWS; i++) {
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      Pos pos = Pos(i, j);
      if (!pieceBoard.isEmpty(pos)) {
        Piece piece = pieceBoard.getPiece(pos);
        int v = colorC[piece.color] * getDistanceToGoalRow(piece, pos);
        distanceValue[piece.color] += v;
      }
    }
  }

  distanceValue[0] /= (pc->whiteDoubles + pc->whiteSingles); distanceValue[1] /= (pc->whiteDoubles + pc->whiteSingles);

  return 10 * pieceValue + distanceValue[0] + distanceValue[1];
}

int GreedyPlayer::decideOnBoard(State state) {
  int ind = -1;
  int max = 0;
  for (int i = 0; i <  state.possiblepieceboards.size(); i++) {
    auto child = state.possiblepieceboards[i];
    int val = evaluateBoard(child);
    if (ind == -1 || val > max) {
      max = val;
      ind = i;
    }
  }
  return ind;
}

