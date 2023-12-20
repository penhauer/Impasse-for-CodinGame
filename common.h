#pragma once
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <math.h>
#include <numeric>
#include <random>
#include <cassert>

const int ROWS = 8;
const int COLS = 8;




const int WHITE = 0;
const int BLACK = 1;
const int UP_DIR = -1;
const int DOWN_DIR = 1;
const int SINGLE_COUNT = 1;
const int DOUBLE_COUNT = 2;
const int BOARD_GAME_ONGOING = -10;
const int BOARD_WHITE_WON = WHITE;
const int BOARD_BLACK_WON = BLACK;

struct Pos {
  short row, col;

  Pos() {}

  Pos(int row, int col) {
    this->row = row;
    this->col = col;
  }

  bool operator==(const Pos& p) const {
    return p.row == row && p.col == col;
  }


  bool operator<(const Pos &other) const {
    if (row < other.row) {
      return true;
    }
    return col < other.col;
  }


  static Pos parseMove(const std::string &s);
  std::string reverseParseMove();
};


const Pos EMPTY_POSE = Pos(-1, -1);

class Piece
{
  private:

  public:
    char c = '.';

    Piece();
    Piece(char c);


    void makeSingle();
    void makeDouble();

    bool isSingle();
    bool isDouble();
    bool IsNoPiece();

    short getDirection() {
      return (c == 'W' || c == 'b' ? DOWN_DIR : UP_DIR);
    }

    short getColor() {
      return (c == 'w' || c == 'W') ? WHITE : BLACK;
    }

    const static char EMPTY_PIECE = '.';
    const static char SINGLE_WHITE = 'w';
    const static char DOUBLE_WHITE = 'W';
    const static char SINGLE_BLACK = 'b';
    const static char DOUBLE_BLACK = 'B';

};

const Piece NO_PIECE = Piece(Piece::EMPTY_PIECE);

struct Move
{
  Pos from = EMPTY_POSE;
  Pos to = EMPTY_POSE;
  Pos remove = EMPTY_POSE;

  Move();
  Move(Pos from, Pos to, Pos remove);

  bool operator<(const Move &other) const;
  bool operator==(const Move &other) const {
    return from == other.from && to == other.to && remove == other.remove;
  }

  std::string toStr();
};


inline int getCrowningRow(int color) {
  return color == WHITE ? 0 : ROWS - 1;
}

inline int getBearOffRow(int color) {
  return color == WHITE ? ROWS - 1 : 0;
}

// can piece crown if it moves to toPos?
inline bool canCrown(Piece piece, Pos toPos) {
  return piece.isSingle() && toPos.row == getCrowningRow(piece.getColor());
}

// can piece bearOff if it moves to toPos?
inline bool canBearOff(Piece piece, Pos toPos) {
  return piece.isDouble() && toPos.row == getBearOffRow(piece.getColor());
}

inline int getDistanceToGoalRow(Piece piece, Pos pos) {
  return piece.getDirection() == UP_DIR ? pos.row : ROWS - 1 - pos.row;
}
