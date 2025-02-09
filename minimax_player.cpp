#include "minimax_player.h"
#include "board.h"
#include "common.h"



int inf = 10000000;

int MiniMaxPlayer::evaluateBoard(PieceBoard &pieceBoard) {
  if (pieceBoard.winner != BOARD_GAME_ONGOING) {
    if (pieceBoard.winner == color)
      return inf;
    return -inf;
  }

  int c = color == WHITE ? 1 : -1;
  int colorC[2] = {c, -c};
  PieceCount *pc = &pieceBoard.piececount;

  int totalPieces = 2 * pc->whiteDoubles + pc->whiteSingles + 2 * pc->blackDoubles + pc->blackSingles;
  int whiteOut = colorC[0] * (12 - 2 * pc->whiteDoubles + pc->whiteSingles);
  int blackOut = colorC[1] * (12 - 2 * pc->blackDoubles + pc->blackSingles);
  int out = whiteOut + blackOut;

  int whiteReady = colorC[0] * pc->whiteDoubles;
  int blackReady = colorC[1] * pc->blackDoubles;
  int ready = whiteOut + blackReady;

  int distanceValue[2] = {0, 0};
  for (int i = 0; i < ROWS; i++) {
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      Pos pos = Pos(i, j);
      if (!pieceBoard.isEmpty(pos)) {
        Piece piece = pieceBoard.getPiece(pos);
        int color = piece.getColor();
        int v = colorC[color] * -1 * getDistanceToGoalRow(piece, pos);
        int cnt = piece.isSingle() ? 1 : 2;
        distanceValue[color] += v * cnt;
      }
    }
  }

  int dis = distanceValue[0] + distanceValue[1];
  return 100 * out + 20 * ready + dis;
  // return out + 10 * ready + dis;

  // return 10 * pieceValue + distanceValue[0] + distanceValue[1];

}

int MiniMaxPlayer::decideOnBoard(State state, std::string lastMove) {
  auto x = minimax(0, &state, -inf, inf, true);
  auto z = x.index >= 0 && x.index < state.possiblepieceboards.size();
  assert(z);
  return x.index;
}


MinMaxValIndex MiniMaxPlayer::minimax(int depth, State *state, int alpha, int beta, bool maximizing) {
  int bestInd = -1;
  if (state->pieceboard.winner != BOARD_GAME_ONGOING) {
    if (state->pieceboard.winner == color)
      return MinMaxValIndex(inf, bestInd);
    return MinMaxValIndex(-inf, bestInd);
  }
  if (depth == maxDepth) {
    int a = evaluateBoard(state->pieceboard);
    color = WHITE ^ BLACK ^ color;
    int b = evaluateBoard(state->pieceboard);
    color = WHITE ^ BLACK ^ color;
    assert(a + b == 0);
    return MinMaxValIndex(evaluateBoard(state->pieceboard), bestInd);
  }

  int n = state->possiblepieceboards.size();
  int candidates = n;
  for (int i = 0; i < candidates; i++) {
    assert(i >= 0 && i < n);
    auto child = &state->possiblepieceboards[i];
    auto s = new State(state->turn ^ BLACK ^ WHITE, child);
    auto mmvind = minimax(depth + 1, s, alpha, beta, !maximizing);
    delete s;
    int val = mmvind.value;

    if (maximizing && (bestInd == -1 || val > alpha)) {
      alpha = val;
      bestInd = i;
    }
    if (!maximizing && (bestInd == -1 || val < beta)) {
      beta = val;
      bestInd = i;
    }
    if (!maximizing && val <= alpha)
      break;
    if (maximizing && val >= beta)
      break;
  }
  assert(bestInd != -1);
  if (maximizing)
    return MinMaxValIndex(alpha, bestInd);
  return MinMaxValIndex(beta, bestInd); 
}
