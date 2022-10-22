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


class Piece
{
public:
    int piece;
    int color;
    int direction;
    int distance;
    int transitions;
    Piece();
    Piece(int piece, int color, int pos);
    void getDirection();
    void changeType();
    void getDistance(const int &pos);
};

struct Move
{
    int from;
    int to;
    int remove;
    Move();
    Move(int from, int to, int remove);
    bool operator<(const Move &other) const;
};

int parseMove(const std::string &notation);
std::string reverseParseMove(int pos);

void printMove(const Move &move);