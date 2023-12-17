#include "board.h"
#include "common.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

PieceBoard::PieceBoard(){
  winner = BOARD_GAME_ONGOING;
  placePieces();
};
// Evaluate board position based on piece count, number of transitions, and distances from the pieces' last rows

int PieceBoard::evaluate(int color) const {
  int coefficient = (color == WHITE ? 1 : -1);
  int otherColor = color ^ WHITE ^ BLACK;

  int piecevalue = coefficient * (2 * piececount.blackSingles + 3 * piececount.blackDoubles - 2 * piececount.whiteSingles - 3 * piececount.whiteDoubles);
  int transitionvalue = transitions[color] - transitions[otherColor];
  // Average distances from last row for each color
  int distancevalue = 0;
  if (color == WHITE)
  {
    int distancevalue = distances[color] / (piececount.whiteSingles + piececount.whiteDoubles + 1) - distances[-otherColor] / (piececount.blackDoubles + piececount.blackSingles + 1);
  }
  else
  {
    int distancevalue = distances[color] / (piececount.blackDoubles + piececount.blackSingles + 1) - distances[-otherColor] / (piececount.whiteSingles + piececount.whiteDoubles + 1);
  };
  return 10 * piecevalue + 6 * transitionvalue - distancevalue;
};

inline int getInd(Pos pos) {
  return (pos.row << 2) + (pos.col >> 1);
}

Piece PieceBoard::getPiece(Pos pos){
  return table[getInd(pos)];
}

void PieceBoard::setPiece(Pos pos, Piece piece) {
  table[getInd(pos)] = piece;
}

void PieceBoard::removePiece(Pos pos) {
  setPiece(pos, NO_PIECE);
}

State::State() {
  pieceboardhistory.clear();
  pieceboard.posToCrown[0] = pieceboard.posToCrown[1] = EMPTY_POSE;

  turn = WHITE;

  generateLegalMoves();
};


void State::printBoard() {
  pieceboard.printBoard();
}


bool State::inside(Pos pos) {
  return pos.row >= 0 && pos.row < ROWS && pos.col >= 0 && pos.col < COLS;
}

// Print board
void PieceBoard::printBoard() {
  for (int row = 0; row < ROWS; row++)
  {
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    std::cout << row + 1 << " ";
    for (int col = 0; col < COLS; col++)
    {
      std::cout << "|";
      Pos pos = Pos(row, col);
      if (pos == lastmove.from) {
        std::cout << "<";
      }
      else if (pos == lastmove.to) {
        std::cout << ">";
      }
      else if (pos == lastmove.remove) {
        std::cout << "x";
      }
      else {
        std::cout << " ";
      }

      if (!isEmpty(pos)) {
        Piece piece = getPiece(pos);
        if (piece.isSingle()) {
          switch (piece.color) {
            case WHITE:
              std::cout << "○";
              break;
            case BLACK:
              std::cout << "◇";
              break;
          }
        } else if (piece.isDouble()) {
          switch (piece.color)
          {
            case WHITE:
              std::cout << "●";
              break;
            case BLACK:
              std::cout << "◆";
              break;
          }
        }
      }
      else if ((row + col) % 2 == 0) {
        std::cout << "/";
      }
      else {
        std::cout << " ";
      }

      if (pos == lastmove.from) {
        std::cout << ">";
      }
      else if (pos == lastmove.to) {
        std::cout << "<";
      }
      else if (pos == lastmove.remove) {
        std::cout << "x";
      }
      else {
        std::cout << " ";
      }
    };
    std::cout << "|" << std::endl;
  };
  std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
  std::cout << "    A   B   C   D   E   F   G   H " << std::endl;
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

void PieceBoard::doSanityCheck() {
  PieceCount pc = PieceCount();
  for (int i = 0; i < ROWS; i++)
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      if (!isEmpty(Pos(i, j))) {
        Piece piece = getPiece(Pos(i, j));
        int d = (piece.isDouble() && piece.color == WHITE || piece.isSingle() && piece.color == BLACK) ? DOWN_DIR : UP_DIR; 
        assert(d == piece.direction);

        if (piece.isSingle()) {
          if (piece.color == WHITE)
            pc.whiteSingles++;
          if (piece.color == BLACK)
            pc.blackSingles++;
        }

        if (piece.isDouble()) {
          if (piece.color == WHITE)
            pc.whiteDoubles++;
          if (piece.color == BLACK)
            pc.blackDoubles++;
        }
      }
    }

  assert(piececount.whiteSingles == pc.whiteSingles);
  assert(piececount.whiteDoubles == pc.whiteDoubles);
  assert(piececount.blackSingles == pc.blackSingles);
  assert(piececount.blackDoubles == pc.blackDoubles);

  int colors[2] = {WHITE, BLACK};
  for (int i = 0; i < 2; i++) {
    int color = colors[i];
    if (!(posToCrown[color] == EMPTY_POSE)) {
      auto pos = posToCrown[color];
      assert(pos.row == 0 || pos.row == ROWS - 1);
      assert(!isEmpty(pos));
      Piece piece = getPiece(pos);
      assert(piece.color == color);
      int r = pos.row + piece.direction;
      assert(r < 0 || r >= ROWS);
      assert(piece.isSingle());
    }
  }


  for (int colorInd = 0; colorInd < 2; colorInd++) {
    int color = colors[colorInd];
    int crowningRow = color == WHITE ? 0 : ROWS - 1;
    for (int col = 0; col < ROWS; col++) {
      Pos pos = Pos(crowningRow, col);
      if (!isEmpty(pos)) {
        Piece piece = getPiece(pos);
        if (piece.color == color && piece.isSingle() && !(posToCrown[color] == EMPTY_POSE)) {
          assert(pos == posToCrown[color]);
        }
      }
    }

    int bearOffRow = color == WHITE ? ROWS - 1 : 0;
    for (int col = 0; col < ROWS; col++) {
      Pos pos = Pos(bearOffRow, col);
      if (!isEmpty(pos)) {
        Piece piece = getPiece(pos);
        if (piece.color == color)
          assert(!piece.isDouble());
      }
    }
  }
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

// Turn single piece into double
void PieceBoard::crown(Pos pos) {
  assert(!isEmpty(pos));
  Piece piece = getPiece(pos);

  switch (piece.color)
  {
    case WHITE:
      piececount.whiteDoubles++;
      piececount.whiteSingles--;
      break;
    case BLACK:
      piececount.blackDoubles++;
      piececount.blackSingles--;
      break;
  }

  if (!(posToCrown[piece.color] == EMPTY_POSE)) {
    Pos toCrownPos = posToCrown[piece.color];
    if (pos == toCrownPos) {
      posToCrown[piece.color] = EMPTY_POSE;
    }
  }

  transitions[piece.color] ++;
  piece.makeDouble();
  setPiece(pos, piece);
}


// Turn double piece into single
void PieceBoard::bearOff(Pos pos) {
  assert(!isEmpty(pos));
  Piece piece = getPiece(pos);
  assert(piece.isDouble());

  switch (piece.color) {
    case WHITE:
      piececount.whiteDoubles--;
      piececount.whiteSingles++;
      break;
    case BLACK:
      piececount.blackDoubles--;
      piececount.blackSingles++;
      break;
  }

  int crowningRow = piece.color == WHITE ? 0 : ROWS - 1;
  if (pos.row == crowningRow) {
    // this happens in case of impasse
    // when removing the top checker of a double piece making it a single piece
    // in this case if there is any single piece in the same row we should crown
    // either this piece or that one. But in the case we're crowning this one
    // we make sure first the other piece in this row is removed and then this one
    // is crowed
    assert(posToCrown[piece.color] == EMPTY_POSE);
    posToCrown[piece.color] = pos;
  }

  piece.makeSingle();
  setPiece(pos, piece);
  transitions[piece.color]++;
}


// Remove / bear-off piece based on its type
void PieceBoard::remove(Pos pos) {

  assert(!isEmpty(pos));
  Piece piece = getPiece(pos);

  if (piece.isSingle()) {
    // Remove single
    switch (piece.color)
    {
      case WHITE:
        piececount.whiteSingles--;
        break;
      case BLACK:
        piececount.blackSingles--;
        break;
    }
    if (!(posToCrown[piece.color] == EMPTY_POSE)) {
      Pos crownPos = posToCrown[piece.color];
      if (crownPos == pos) {
        posToCrown[piece.color] = EMPTY_POSE;
      }
    }
    removePiece(pos);
  } else if (piece.isDouble()) {
    // Bear-off double
    bearOff(pos);
  }

  // pieceboard.distances[p.color] -= p.distance;
  if (piececount.whiteSingles + piececount.whiteDoubles == 0) {
    winner = BOARD_WHITE_WON;
  } else if (piececount.blackSingles + piececount.blackDoubles == 0) {
    winner = BOARD_BLACK_WON;
  }
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
  if (abs(pos.col - toPos.col) != 1 || (pos.row + piece.direction) != toPos.row) {
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

// Return true if position is empty
bool PieceBoard::isEmpty(Pos pos) {
  Piece p = getPiece(pos);
  return p.pieceCount == NO_PIECE.pieceCount;
}

// can piece crown if it moves to toPos?
bool canCrown(Piece piece, Pos toPos) {
  int crowningRow = piece.color == WHITE ? 0 : ROWS - 1;
  return piece.isSingle() && toPos.row == crowningRow;
}

// can piece bearOff if it moves to toPos?
bool canBearOff(Piece piece, Pos toPos) {
  int bearOffRow = piece.color == WHITE ? ROWS - 1: 0;
  return piece.isDouble() && toPos.row == bearOffRow;
}


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
  int dr = piece.direction;
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

// Move piece by transposing with the target square
void PieceBoard::move(Pos pos, Pos toPos) {
  // If square is not empty, change places (in case of transpose)
  if (!isEmpty(toPos)) {
    Piece currentPiece = getPiece(pos);
    Piece toPiece = getPiece(toPos);
    setPiece(pos, toPiece);
    setPiece(toPos, currentPiece);
  }
  // Else just move the piece
  else {
    Piece currentPiece = getPiece(pos);
    setPiece(toPos, currentPiece);
    removePiece(pos);
  }
  // int distancediff = pieceboard.piecemap.at(toPos).getDistance(toPos);
  // pieceboard.distances[turn] += distancediff;
}


// Check if there's a piece waiting to be crowned, and if so, crown it with the current piece
Pos PieceBoard::crownIf(Pos pos) {
  bool crowned = false;
  assert(!isEmpty(pos));
  Piece piece = getPiece(pos);
  if (!(posToCrown[piece.color] == EMPTY_POSE)) {
    Pos crownpiecepos = posToCrown[piece.color];
    crown(crownpiecepos);
    remove(pos);
    return crownpiecepos;
  }
  return EMPTY_POSE;
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


// Create new board
void PieceBoard::placePieces() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      removePiece(Pos(i, j));
    }
  }

  Pos poses[16] = {
    Pos(0, 1),
    Pos(0, 5),
    Pos(1, 2),
    Pos(1, 6),

    Pos(6, 3),
    Pos(6, 7),
    Pos(7, 0),
    Pos(7, 4),

    Pos(0, 3),
    Pos(0, 7),
    Pos(1, 0),
    Pos(1, 4),

    Pos(6, 1),
    Pos(6, 5),
    Pos(7, 2),
    Pos(7, 6),
  };

  Piece pieces[16] = {
    Piece(DOUBLE_COUNT, WHITE),
    Piece(DOUBLE_COUNT, WHITE),
    Piece(DOUBLE_COUNT, WHITE),
    Piece(DOUBLE_COUNT, WHITE),

    Piece(SINGLE_COUNT, WHITE),
    Piece(SINGLE_COUNT, WHITE),
    Piece(SINGLE_COUNT, WHITE),
    Piece(SINGLE_COUNT, WHITE),

    Piece(SINGLE_COUNT, BLACK),
    Piece(SINGLE_COUNT, BLACK),
    Piece(SINGLE_COUNT, BLACK),
    Piece(SINGLE_COUNT, BLACK),

    Piece(DOUBLE_COUNT, BLACK),
    Piece(DOUBLE_COUNT, BLACK),
    Piece(DOUBLE_COUNT, BLACK),
    Piece(DOUBLE_COUNT, BLACK),
  };

  for (int i = 0; i < 16; i++) {
    setPiece(poses[i], pieces[i]);
  }

  piececount.whiteDoubles = 4;
  piececount.whiteSingles = 4;

  piececount.blackSingles = 4;
  piececount.blackDoubles = 4;
}

