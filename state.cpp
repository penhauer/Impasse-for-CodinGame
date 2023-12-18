#include "state.h"

State::State() {
  pieceboardhistory.clear();
  pieceboard.posToCrown[0] = pieceboard.posToCrown[1] = EMPTY_POSE;

  turn = WHITE;

  generateLegalMoves();
}


// Generate legal moves for the player in turn
void State::generateLegalMoves() {
  possiblepieceboards.clear();
  for (int i = 0; i < ROWS; i++)
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      Pos pos = Pos(i, j);
      if (!pieceboard.isEmpty(pos)) {
        Piece piece = pieceboard.getPiece(pos);
        if (piece.color == turn) {
          addPieceMoves(pos);
        }
      }
    }

  if (possiblepieceboards.empty()) {
    // If no moves are avilable, add impasse moves
    addImpassable();
  }
}

// Update board state with selected move
void State::doMove(int moveNumber) {
  PieceBoard nextPieceBoard = possiblepieceboards[moveNumber];
  doMove(nextPieceBoard);
}

void State::doMove(const PieceBoard nextPieceBoard) {
  pieceboardhistory.push_back(pieceboard);
  pieceboard = nextPieceBoard;
  changeTurn();
  generateLegalMoves();
}


// Undo last move
void State::undoMove() {
  pieceboard = pieceboardhistory.back();
  pieceboardhistory.pop_back();
  changeTurn();
  generateLegalMoves();
}

void State::changeTurn() {
  this->turn ^= WHITE ^ BLACK;
}



void State::printBoard() {
  pieceboard.printBoard();
}


bool State::inside(Pos pos) {
  return pos.row >= 0 && pos.row < ROWS && pos.col >= 0 && pos.col < COLS;
}


// Create set of singles color matching the player in turn for crowning
std::vector<Pos> State::checkSingles(Pos currentPiecePos)
{
  assert(!pieceboard.isEmpty(currentPiecePos));
  Piece currentPiece = pieceboard.getPiece(currentPiecePos);
  std::vector<Pos> singles;
  for (int i = 0; i < ROWS; i++)
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      Pos pos = Pos(i, j);
      if (!pieceboard.isEmpty(pos)) {
        Piece p = pieceboard.getPiece(pos);
        if (p.isSingle() && p.color == currentPiece.color && !(pos == currentPiecePos)) {
          singles.push_back(pos);
        }
      }
    }
  return singles;
}


bool State::isTransposable(Pos pos, Pos toPos) {
  if (pieceboard.isEmpty(pos) || pieceboard.isEmpty(toPos) ) {
    return false;
  }
  Piece piece = pieceboard.getPiece(pos);
  Piece toPiece = pieceboard.getPiece(toPos);
  if (abs(pos.col - toPos.col) != 1 || (pos.row + piece.getDirection()) != toPos.row) {
    return false;
  }
  if (toPiece.color != piece.color) {
    return false;
  }

  if (piece.isSingle() && toPiece.isDouble() || (piece.isDouble() && toPiece.isSingle())) {
  } else {
    return false;
  }
  if (pos.row > 0 && pos.row < ROWS - 1 && toPos.row > 0 && toPos.row < ROWS - 1) {
    // A transpose is limited this way from only one side.
    return piece.isDouble() && toPiece.isSingle();
  }
  if (toPos.row != 0 && toPos.row != ROWS - 1) {
    return false;
  }
  return (piece.isSingle() && toPiece.isDouble()) || (piece.isDouble() && toPiece.isSingle());
}



void State::checkImpasseForPos(Pos pos) {
  Piece piece = pieceboard.getPiece(pos);
  // If piece is double, check if it's first row, and a single is waiting to be crowned.
  // If so, in case of impasse of this double, 2 crownings are made possible.
  // If not, just add the double as a normal impasse move.
  if (piece.isDouble()) {
    if ((pos.row == 0 || pos.row == ROWS - 1) && !(pieceboard.posToCrown[piece.color] == EMPTY_POSE))
    {
      PieceBoard new_pieceboard = pieceboard;
      Pos removepiecepos = pieceboard.posToCrown[piece.color];
      new_pieceboard.remove(removepiecepos);
      new_pieceboard.remove(pos);
      new_pieceboard.crown(pos);
      new_pieceboard.lastmove = Move(pos, EMPTY_POSE, removepiecepos);
      possiblepieceboards.push_back(new_pieceboard);

      // TODO: add other way around
    } else {
      PieceBoard new_pieceboard = pieceboard;
      new_pieceboard.remove(pos);
      Pos crownedPiecePos = new_pieceboard.crownIf(pos);
      new_pieceboard.lastmove = Move(pos, crownedPiecePos, pos);
      possiblepieceboards.push_back(new_pieceboard);
    }
  } // If piece is single, no need to check for crowning, since then wouldn't be a single in the first place
  else {
    PieceBoard new_pieceboard = pieceboard;
    new_pieceboard.remove(pos);
    new_pieceboard.lastmove = Move(pos, EMPTY_POSE, pos);
    possiblepieceboards.push_back(new_pieceboard);
  }
}

// Add all legal impasse moves
void State::addImpassable() {
  for (int i = 0; i < ROWS; i++)
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      Pos pos = Pos(i, j);
      if (!pieceboard.isEmpty(pos)) {
        Piece piece = pieceboard.getPiece(pos);
        if (piece.color == turn) {
          checkImpasseForPos(pos);
        }
      }
    }
};



void State::checkBearOff(Pos pos, Pos toPos) {
  assert(!pieceboard.isEmpty(pos));
  Piece piece = pieceboard.getPiece(pos);
  if (canBearOff(piece, toPos)){
    PieceBoard new_pieceboard = pieceboard;
    new_pieceboard.move(pos, toPos);
    new_pieceboard.remove(toPos);
    Pos crownedPiecePos = new_pieceboard.crownIf(toPos);
    if (!(crownedPiecePos == EMPTY_POSE)) {
      new_pieceboard.lastmove = Move(pos, toPos, pos);
    } else {
      new_pieceboard.lastmove = Move(pos, toPos, EMPTY_POSE);
    }
    possiblepieceboards.push_back(new_pieceboard);
  }
}


void State::checkCrown(Pos pos, Pos toPos) {
  assert(!pieceboard.isEmpty(pos));
  Piece piece = pieceboard.getPiece(pos);
  if (canCrown(piece, toPos)) {
    auto singles = checkSingles(pos);
    // If other single available
    if (singles.size() > 0) {
      for (auto removepiecepos : singles) {
        PieceBoard new_pieceboard = pieceboard;
        new_pieceboard.lastmove = Move(pos, toPos, removepiecepos);
        new_pieceboard.move(pos, toPos);
        new_pieceboard.crown(toPos);
        new_pieceboard.remove(removepiecepos);
        possiblepieceboards.push_back(new_pieceboard);
      }
    } else {
      // if no other singles available, just add as normal slide (and add to crownstack)
      PieceBoard new_pieceboard = pieceboard;
      new_pieceboard.lastmove = Move(pos, toPos, EMPTY_POSE);
      new_pieceboard.move(pos, toPos);
      new_pieceboard.posToCrown[piece.color] = toPos;
      possiblepieceboards.push_back(new_pieceboard);
    }
  }
}


void State::doSimpleMove(Pos pos, Pos toPos) {
  PieceBoard new_pieceboard = pieceboard;
  new_pieceboard.lastmove = Move(pos, toPos, EMPTY_POSE);
  new_pieceboard.move(pos, toPos);
  possiblepieceboards.push_back(new_pieceboard);
}


// Add legal moves for a piece
void State::addPieceMoves(Pos pos) {
  assert(!pieceboard.isEmpty(pos));
  Piece piece = pieceboard.getPiece(pos);

  // Separate left and right search
  int dr = piece.getDirection();
  for (int dc = -1; dc <= 1; dc += 2) {
    for (int steps = 1; ; steps++) {
      int toRow = pos.row + dr * steps;
      int toCol = pos.col + dc * steps;
      Pos toPos = Pos(toRow, toCol);
      if (!inside(toPos)) {
        break;
      }
      // Slide or transpose
      bool transposable = isTransposable(pos, toPos);
      bool emptysquare = pieceboard.isEmpty(toPos);
      if (emptysquare || transposable) {
        if (toRow == 0 || toRow == ROWS - 1) {
          checkCrown(pos, toPos);
          checkBearOff(pos, toPos);
        } else {
          doSimpleMove(pos, toPos);
        }
        // if it was a transpose, break loop with current sign
        if (transposable) {
          break;
        }
      } else {
        // If the square was not empty or transposable, break the loop
        break;
      }
    }
  }
}

