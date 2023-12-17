#pragma once
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <tuple>
#include <set>
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




const int BLACK = -1;
const int WHITE = 1;
const int UP_DIR = -1;
const int DOWN_DIR = 1;
const int SINGLE_COUNT = 1;
const int DOUBLE_COUNT = 2;

struct Pos {
  int row, col;

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
  public:
    int pieceCount;
    int color;
    int direction;
    int distance;

    Piece();
    Piece(int pieceCount, int color);


    void makeSingle();
    void makeDouble();

    int getDistance(const Pos &pos);

    bool isSingle();
    bool isDouble();


  private:
    void setDirection();
};

const Piece NO_PIECE = Piece(-1, -1);

struct Move
{
  Pos from = EMPTY_POSE;
  Pos to = EMPTY_POSE;
  Pos remove = EMPTY_POSE;
  Pos crown = EMPTY_POSE;
  Pos bearoff = EMPTY_POSE;

  Move();
  Move(Pos from, Pos to, Pos remove);

  bool operator<(const Move &other) const;
  bool operator==(const Move &other) const {
    return from == other.from && to == other.to && remove == other.remove;
  }

  std::string toStr();
};

