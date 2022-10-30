#pragma once
#include "board.h"

struct TranspositionEntry
{
    int type;
    int score;
    int depth;
};

typedef uint64_t HashValue;
typedef std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, HashValue>>> ZobristTable;
typedef std::unordered_map<HashValue, TranspositionEntry> TranspositionTable;

class Ai
{
private:
    int color;
    int leafnodes;
    std::map<int,int> cutoffs;
    ZobristTable zobristtable;
    TranspositionTable tt;

public:
    Ai();
    Ai(int color);
    PieceBoard getMove(Board board, const int &aitime);

private:
    std::tuple<int, PieceBoard> alphaBetaNegaMax(Board board, int depth, int color, int alpha, int beta);
    void orderMoves(PieceBoardVector &childnodes, const int &color);
    void initZobristTable();
    HashValue getHashValue(const PieceBoard &pb, const int &turn);
    void addCutoff(const int &depth);
};