#pragma once
#include "board.h"
#include "player.h"
#include <vector>

struct TranspositionEntry
{
    int type;
    int score;
    int depth;
};

typedef uint64_t HashValue;
typedef std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, HashValue>>> ZobristTable;
typedef std::unordered_map<HashValue, TranspositionEntry> TranspositionTable;

class Ai : public Player
{
private:
    int color;
    int leafnodes;
    int AITime;
    std::map<int,int> cutoffs;
    ZobristTable zobristtable;
    TranspositionTable tt;

public:
    Ai(int color, int AITime);
    PieceBoard getMove(State state);


    int decideOnBoard(State state) override;

private:
    std::tuple<int, PieceBoard> alphaBetaNegaMax(State state, int depth, int color, int alpha, int beta);
    void orderMoves(std::vector<PieceBoard> &childnodes, const int &color);
    void initZobristTable();
    HashValue getHashValue(PieceBoard &pb, const int &turn);
    void addCutoff(const int &depth);
};
