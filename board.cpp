#include "board.h"
#include "common.h"
#include <cassert>
#include <iostream>

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
  if ((pos.row + pos.col) % 2 == 0)
    return NO_PIECE;
  return table[getInd(pos)];
}

void PieceBoard::setPiece(Pos pos, Piece piece) {
  table[getInd(pos)] = piece;
}

void PieceBoard::removePiece(Pos pos) {
  setPiece(pos, NO_PIECE);
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
          switch (piece.getColor()) {
            case WHITE:
              std::cout << "○";
              break;
            case BLACK:
              std::cout << "◇";
              break;
          }
        } else if (piece.isDouble()) {
          switch (piece.getColor())
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


void PieceBoard::doSanityCheck() {
  PieceCount pc = PieceCount();
  for (int i = 0; i < ROWS; i++)
    for (int j = (1 - i % 2); j < COLS; j += 2) {
      if (!isEmpty(Pos(i, j))) {
        Piece piece = getPiece(Pos(i, j));
        int d = (piece.isDouble() && piece.getColor() == WHITE || piece.isSingle() && piece.getColor() == BLACK) ? DOWN_DIR : UP_DIR; 
        assert(d == piece.getDirection());

        if (piece.isSingle()) {
          if (piece.getColor() == WHITE)
            pc.whiteSingles++;
          if (piece.getColor() == BLACK)
            pc.blackSingles++;
        }

        if (piece.isDouble()) {
          if (piece.getColor() == WHITE)
            pc.whiteDoubles++;
          if (piece.getColor() == BLACK)
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
      assert(piece.getColor() == color);
      int r = pos.row + piece.getDirection();
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
        if (piece.getColor() == color && piece.isSingle() && !(posToCrown[color] == EMPTY_POSE)) {
          assert(pos == posToCrown[color]);
        }
      }
    }

    int bearOffRow = color == WHITE ? ROWS - 1 : 0;
    for (int col = 0; col < ROWS; col++) {
      Pos pos = Pos(bearOffRow, col);
      if (!isEmpty(pos)) {
        Piece piece = getPiece(pos);
        if (piece.getColor() == color)
          assert(!piece.isDouble());
      }
    }
  }
}

// Turn single piece into double
void PieceBoard::crown(Pos pos) {
  assert(!isEmpty(pos));
  Piece piece = getPiece(pos);

  switch (piece.getColor())
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

  int color = piece.getColor();
  if (!(posToCrown[color] == EMPTY_POSE)) {
    Pos toCrownPos = posToCrown[color];
    if (pos == toCrownPos) {
      posToCrown[color] = EMPTY_POSE;
    }
  }

  piece.makeDouble();
  setPiece(pos, piece);
}


// Turn double piece into single
void PieceBoard::bearOff(Pos pos) {
  assert(!isEmpty(pos));
  Piece piece = getPiece(pos);
  assert(piece.isDouble());

  int color = piece.getColor();
  switch (color) {
    case WHITE:
      piececount.whiteDoubles--;
      piececount.whiteSingles++;
      break;
    case BLACK:
      piececount.blackDoubles--;
      piececount.blackSingles++;
      break;
  }

  int crowningRow = color == WHITE ? 0 : ROWS - 1;
  if (pos.row == crowningRow) {
    // this happens in case of impasse
    // when removing the top checker of a double piece making it a single piece
    // in this case if there is any single piece in the same row we should crown
    // either this piece or that one. But in the case we're crowning this one
    // we make sure first the other piece in this row is removed and then this one
    // is crowed
    assert(posToCrown[color] == EMPTY_POSE);
    posToCrown[color] = pos;
  }

  piece.makeSingle();
  setPiece(pos, piece);
  transitions[color]++;
}


// Remove / bear-off piece based on its type
void PieceBoard::remove(Pos pos) {

  assert(!isEmpty(pos));
  Piece piece = getPiece(pos);
  int color = piece.getColor();

  if (piece.isSingle()) {
    // Remove single
    switch (color)
    {
      case WHITE:
        piececount.whiteSingles--;
        break;
      case BLACK:
        piececount.blackSingles--;
        break;
    }
    if (!(posToCrown[color] == EMPTY_POSE)) {
      Pos crownPos = posToCrown[color];
      if (crownPos == pos) {
        posToCrown[color] = EMPTY_POSE;
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





// Return true if position is empty
bool PieceBoard::isEmpty(Pos pos) {
  Piece p = getPiece(pos);
  return p.IsNoPiece();
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
  int color = piece.getColor();
  if (!(posToCrown[color] == EMPTY_POSE)) {
    Pos crownpiecepos = posToCrown[color];
    crown(crownpiecepos);
    remove(pos);
    return crownpiecepos;
  }
  return EMPTY_POSE;
}



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

