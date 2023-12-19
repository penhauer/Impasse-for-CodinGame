#include "minimax_player.h"
#include "board.h"
#include "common.h"
#include <algorithm>
#include <numeric>



int inf = 100000;

int MiniMaxPlayer::evaluateBoard(PieceBoard &pieceBoard) {
  if (pieceBoard.winner != BOARD_GAME_ONGOING) {
    if (pieceBoard.winner == color)
      return inf;
    return -inf;
  }


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
        int color = piece.getColor();
        int v = colorC[color] * getDistanceToGoalRow(piece, pos);
        distanceValue[color] += v;
      }
    }
  }

  distanceValue[0] /= (pc->whiteDoubles + pc->whiteSingles); distanceValue[1] /= (pc->whiteDoubles + pc->whiteSingles);

  return 10 * pieceValue + distanceValue[0] + distanceValue[1];
}

int MiniMaxPlayer::decideOnBoard(State state) {
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
    return MinMaxValIndex(evaluateBoard(state->pieceboard), bestInd);
  }

  int n = state->possiblepieceboards.size();
  int *arr = new int[n];
  std::iota(arr, arr + n, 0);
  std::random_shuffle(arr, arr + n);
  int candidates = std::max(n - depth * 5, std::min(10, n));

  for (int i = 0; i < candidates; i++) {
    int ind = arr[i];
    assert(ind >= 0 && ind < n);
    auto child = state->possiblepieceboards[ind];
    auto s = new State(state->turn ^ BLACK ^ WHITE, &child);
    auto mmvind = minimax(depth + 1, s, alpha, beta, !maximizing);
    delete s;
    int val = mmvind.value;

    if (maximizing && val >= alpha) {
      alpha = val;
      bestInd = ind;
    }
    if (!maximizing && val <= beta) {
      beta = val;
      bestInd = ind;
    }
    if (!maximizing && val <= alpha)
      break;
    if (maximizing && val >= beta)
      break;
  }
  if (maximizing)
    return MinMaxValIndex(alpha, bestInd);
  return MinMaxValIndex(beta, bestInd); 
}
