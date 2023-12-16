#include "board.h"
#include "common.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

PieceBoard::PieceBoard(){};
// Evaluate board position based on piece count, number of transitions, and distances from the pieces' last rows

int PieceBoard::evaluate(int color) const {
    int piecevalue = color * (2 * piececount.blackSingles + 3 * piececount.blackDoubles - 2 * piececount.whiteSingles - 3 * piececount.whiteDoubles);
    int transitionvalue = transitions.at(color) - transitions.at(-color);
    // Average distances from last row for each color
    int distancevalue = 0;
    if (color == 1)
    {
        int distancevalue = distances.at(color) / (piececount.whiteSingles + piececount.whiteDoubles + 1) - distances.at(-color) / (piececount.blackDoubles + piececount.blackSingles + 1);
    }
    else
    {
        int distancevalue = distances.at(color) / (piececount.blackDoubles + piececount.blackSingles + 1) - distances.at(-color) / (piececount.whiteSingles + piececount.whiteDoubles + 1);
    };
    return 10 * piecevalue + 6 * transitionvalue - distancevalue;
};

Piece PieceBoard::getPiece(Pos pos){
  return table[pos.row][pos.col];
}

void PieceBoard::setPiece(Pos pos, Piece piece) {
  table[pos.row][pos.col] = piece;
}

void PieceBoard::removePiece(Pos pos) {
  setPiece(pos, NO_PIECE);
}

Board::Board() {
  pieceboardhistory.clear();
  pieceboard.postocrown.clear();

  turn = WHITE;
  winner = BOARD_GAME_ONGOING;

  newBoard();
  generateLegalMoves();
};


void Board::printBoard() {
  pieceboard.printBoard();
}


bool Board::inside(Pos pos) {
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
void Board::generateLegalMoves() {
    possiblepieceboards.clear();
    for (int i = 0; i < ROWS; i++)
      for (int j = 0; j < COLS; j++) {
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
void Board::doMove(int moveNumber) {
  PieceBoard nextPieceBoard = possiblepieceboards[moveNumber];
  pieceboardhistory.push_back(nextPieceBoard);
  pieceboard = nextPieceBoard;
  changeTurn();
  generateLegalMoves();
}

void Board::doMove(const PieceBoard nextPieceBoard) {
    pieceboardhistory.push_back(pieceboard);
    pieceboard = nextPieceBoard;
    changeTurn();
    generateLegalMoves();
}

// Undo last move
void Board::undoMove() {
    pieceboard = pieceboardhistory.back();
    pieceboardhistory.pop_back();
    changeTurn();
    generateLegalMoves();
}

void Board::changeTurn() {
  this->turn ^= WHITE ^ BLACK;
}

// Turn single piece into double
void Board::crown(PieceBoard &pieceboard, Pos pos) {
  assert(!pieceboard.isEmpty(pos));
  Piece piece = pieceboard.getPiece(pos);
    
    switch (piece.color)
    {
    case WHITE:
        pieceboard.piececount.whiteDoubles++;
        pieceboard.piececount.whiteSingles--;
        break;
    case BLACK:
        pieceboard.piececount.blackDoubles++;
        pieceboard.piececount.blackSingles--;
        break;
    };
    pieceboard.transitions[piece.color] ++;
    piece.pieceCount = DOUBLE_COUNT;
    pieceboard.setPiece(pos, piece);
    piece.setDirection();
}


// Turn double piece into single
void Board::bearOff(PieceBoard &pieceboard, Pos pos) {
    assert(!pieceboard.isEmpty(pos));
    Piece piece = pieceboard.getPiece(pos);
    assert(piece.isDouble());

    switch (piece.color) {
    case WHITE:
        pieceboard.piececount.whiteDoubles--;
        pieceboard.piececount.whiteSingles++;
        break;
    case BLACK:
        pieceboard.piececount.blackDoubles--;
        pieceboard.piececount.blackSingles++;
        break;
    };
    piece.pieceCount = SINGLE_COUNT;
    piece.setDirection();
    pieceboard.setPiece(pos, piece);
    pieceboard.transitions[piece.color]++;
}


// Remove / bear-off piece based on its type
void Board::remove(PieceBoard &pieceboard, Pos pos) {

  assert(!pieceboard.isEmpty(pos));
  Piece piece = pieceboard.getPiece(pos);

  if (piece.isSingle()) {
    // Remove single
    switch (piece.color)
    {
    case WHITE:
        pieceboard.piececount.whiteSingles--;
        break;
    case BLACK:
        pieceboard.piececount.blackSingles--;
        break;
    };
    pieceboard.removePiece(pos);
  } else if (piece.isDouble()) {
    // Bear-off double
    bearOff(pieceboard, pos);
  }

    // pieceboard.distances[p.color] -= p.distance;


    if (pieceboard.piececount.whiteSingles + pieceboard.piececount.whiteDoubles == 0) {
        winner = BOARD_WHITE_WON;
    } else if (pieceboard.piececount.blackSingles + pieceboard.piececount.blackDoubles == 0) {
        winner = BOARD_BLACK_WON;
    }
}

// Create set of singles color matching the player in turn for crowning
PosSet Board::checkSingles(Pos currentPiecePos)
{
  assert(!pieceboard.isEmpty(currentPiecePos));
  Piece currentPiece = pieceboard.getPiece(currentPiecePos);
  PosSet singles;
  // std::vector<Pos> singles;
  for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLS; j++) {
      Pos pos = Pos(i, j);
      if (!pieceboard.isEmpty(pos)) {
        Piece p = pieceboard.getPiece(pos);
        if (p.isSingle() && p.color == currentPiece.color && !(pos == currentPiecePos)) {
          singles.insert(pos);
          // singles.push_back(pos);
        }
      }
    }
  return singles;
}


bool Board::isTransposable(Pos pos, Pos toPos) {
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


// Add legal moves for a piece
void Board::addPieceMoves(Pos pos) {
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
                // Crowning
                if (toRow == 0 || toRow == ROWS - 1) {
                    if (piece.isSingle()) {
                        PosSet singles = checkSingles(pos);
                        // If other single available
                        if (singles.size() > 0) {
                            for (auto removepiecepos : singles)
                            {
                                PieceBoard new_pieceboard = pieceboard;
                                new_pieceboard.lastmove = Move(pos, toPos, removepiecepos);
                                move(new_pieceboard, pos, toPos);
                                crown(new_pieceboard, toPos);
                                remove(new_pieceboard, removepiecepos);
                                possiblepieceboards.push_back(new_pieceboard);
                            }
                        } else {
                          // if no other singles available, just add as normal slide (and add to crownstack)
                            PieceBoard new_pieceboard = pieceboard;
                            new_pieceboard.lastmove = Move(pos, toPos, EMPTY_POSE);
                            move(new_pieceboard, pos, toPos);
                            new_pieceboard.postocrown[turn] = toPos;
                            possiblepieceboards.push_back(new_pieceboard);
                        }
                    }
                    else {
                        PieceBoard new_pieceboard = pieceboard;
                        bearOff(new_pieceboard, pos);
                        bool crowned = crownIf(new_pieceboard, pos);
                        if (!crowned) {
                            new_pieceboard.lastmove = Move(pos, toPos, pos);
                            move(new_pieceboard, pos, toPos);
                        } else {
                            new_pieceboard.lastmove = Move(pos, toPos, EMPTY_POSE);
                        }
                        possiblepieceboards.push_back(new_pieceboard);
                    }
                }
                else {
                    PieceBoard new_pieceboard = pieceboard;
                    new_pieceboard.lastmove = Move(pos, toPos, EMPTY_POSE);
                    move(new_pieceboard, pos, toPos);
                    possiblepieceboards.push_back(new_pieceboard);
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
void Board::move(PieceBoard &pieceboard, Pos pos, Pos toPos) {
    // If square is not empty, change places (in case of transpose)
    if (!pieceboard.isEmpty(toPos)) {
        Piece currentPiece = pieceboard.getPiece(pos);
        Piece toPiece = pieceboard.getPiece(toPos);

        pieceboard.setPiece(pos, toPiece);
        pieceboard.setPiece(toPos, currentPiece);

        // pieceboard.piecemap[toPos] = pieceboard.piecemap.at(pos);
        // pieceboard.piecemap[pos] = toPiece;
        // int distancediff = pieceboard.piecemap.at(pos).getDistance(pos);
        // pieceboard.distances[turn] += distancediff;
    }
    // Else just move the piece
    else {
        Piece currentPiece = pieceboard.getPiece(pos);
        pieceboard.setPiece(toPos, currentPiece);
        pieceboard.removePiece(pos);

        // pieceboard.piecemap[toPos] = pieceboard.piecemap.at(pos);
        // pieceboard.piecemap.erase(pos);
    }
    // int distancediff = pieceboard.piecemap.at(toPos).getDistance(toPos);
    // pieceboard.distances[turn] += distancediff;
}


// Check if there's a piece waiting to be crowned, and if so, crown it with the current piece
bool Board::crownIf(PieceBoard &pieceboard, Pos pos) {
    bool crowned = false;
    if (pieceboard.postocrown.count(turn) > 0) {
        // const Piece &removepiece = pieceboard.piecemap.at(pos);
        // const int &crownpiecepos = pieceboard.postocrown.at(turn);
        // const Piece &crownpiece = pieceboard.piecemap.at(crownpiecepos);
        // crown(pieceboard, crownpiece, crownpiecepos);
        // pieceboard.postocrown.erase(turn);
        // remove(pieceboard, removepiece, pos);
        // crowned = true;

        Pos crownpiecepos = pieceboard.postocrown.at(turn);
        crown(pieceboard, crownpiecepos);
        pieceboard.postocrown.erase(turn);
        remove(pieceboard, pos);
        crowned = true;
    }
    return crowned;
}


bool canBearOff(Pos pos, Piece piece) {
  return piece.isDouble() &&
    (pos.row == ROWS - 1 && piece.color == WHITE || pos.row == 0 && piece.color == BLACK);
}

void Board::checkImpasseForPos(Pos pos) {
  Piece piece = pieceboard.getPiece(pos);

  PieceBoard new_pieceboard = pieceboard;

  // If piece is double, check if it's first row, and a single is waiting to be crowned.
  // If so, in case of impasse of this double, 2 crownings are made possible.
  // If not, just add the double as a normal impasse move.
  if (piece.isDouble()) {
      if ((pos.row == 0 || pos.row == 7) && pieceboard.postocrown.count(turn) > 0)
      {
          Pos removepiecepos = pieceboard.postocrown.at(turn);
          // Piece &removepiece = pieceboard.piecemap.at(removepiecepos);
          remove(new_pieceboard, removepiecepos);
          new_pieceboard.postocrown.erase(turn);
          crown(new_pieceboard, pos);
          new_pieceboard.lastmove = Move(pos, EMPTY_POSE, removepiecepos);
          possiblepieceboards.push_back(new_pieceboard);
          new_pieceboard = pieceboard;
      };
      remove(new_pieceboard, pos);
      crownIf(new_pieceboard, pos);
      new_pieceboard.lastmove = Move(pos, EMPTY_POSE, pos);
  } // If piece is single, no need to check for crowning, since then wouldn't be a single in the first place
  else {
      remove(new_pieceboard, pos);
      new_pieceboard.lastmove = Move(pos, EMPTY_POSE, pos);
  }
  possiblepieceboards.push_back(new_pieceboard);
}

// Add all legal impasse moves
void Board::addImpassable() {
  for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLS; j++) {
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
void Board::newBoard() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      pieceboard.removePiece(Pos(i, j));
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
    pieceboard.setPiece(poses[i], pieces[i]);
  }

  pieceboard.piececount.whiteDoubles = 4;
  pieceboard.piececount.whiteSingles = 4;

  pieceboard.piececount.blackSingles = 4;
  pieceboard.piececount.blackDoubles = 4;
}
