#include "common.h"
#include <string>

Piece::Piece(){};
Piece::Piece(int pieceCount, int color) : pieceCount(pieceCount), color(color)
{
    setDirection();
    // getDistance(pos);
};

bool Piece::isSingle() {
  return pieceCount == SINGLE_COUNT;
}

bool Piece::isDouble() {
  return pieceCount == DOUBLE_COUNT;
}

// Update the direction of the piece
void Piece::setDirection() { 
  direction = (isDouble() && color == WHITE || isSingle() && color == BLACK) ? DOWN_DIR : UP_DIR; 
}

// Update the distance of the piece from its last row
int Piece::getDistance(const Pos &pos) {
    const int olddistance = distance;
    distance = (direction == DOWN_DIR) ? ROWS - pos.row : pos.row;
    return distance - olddistance;
};

void Piece::makeSingle() {
  pieceCount = SINGLE_COUNT;
  setDirection();
}

void Piece::makeDouble() {
  pieceCount = DOUBLE_COUNT;
  setDirection();
}



Move::Move() {
    from = EMPTY_POSE;
    to = EMPTY_POSE;
    remove = EMPTY_POSE;
};

Move::Move(Pos from, Pos to, Pos remove) : from(from), to(to), remove(remove){};

bool Move::operator<(const Move &other) const { 
  return from < other.from && to < other.to && remove < other.remove; 
}

// Convert chess notation to board position
Pos Pos::parseMove(const std::string &s) {
    if (s.size() != 2) {
        return EMPTY_POSE;
    }
    char letter = tolower(s[0]);
    int row = s[1] - '1';
    int col = (int)letter - 97;
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
      return EMPTY_POSE;
    }
    return Pos(row, col);
};

// Convert board position to chess notation
std::string Pos::reverseParseMove() {
    char letter = (char)(col + 'A');
    char number = (char)(row + '1');
    std::string notation = "";
    notation += letter;
    notation += number;
    return notation;
}

std::string Move::toStr() {
  std::string s = "From "  + from.reverseParseMove();
  if (!(to == EMPTY_POSE)) {
      s += " to " + to.reverseParseMove();
  }
  if (!(remove == EMPTY_POSE)) {
      s += " removing " + remove.reverseParseMove();
  }
  return s;
}

