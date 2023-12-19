#include "common.h"
#include <cctype>
#include <string>

Piece::Piece() {
  c = '.';
}

Piece::Piece(char c) {
  this->c = c;
}

bool Piece::isSingle() {
  return c == 'w' || c == 'b';
}

bool Piece::isDouble() {
  return c == 'W' || c == 'B';
}

void Piece::makeSingle() {
  c = tolower(c);
}

void Piece::makeDouble() {
  c = toupper(c);
}

bool Piece::IsNoPiece() {
  return c == '.';
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
  int row = ROWS - 1 - (s[1] - '1');
  int col = letter - 'a';
  if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
    return EMPTY_POSE;
  }
  return Pos(row, col);
};

// Convert board position to chess notation
std::string Pos::reverseParseMove() {
  char letter = (char)(col + 'a');
  char number = (char)(ROWS - 1 - row + '1');
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

