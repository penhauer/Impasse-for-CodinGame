#pragma once
#include <tuple>
#include <unordered_map>

struct TranspositionEntry
{
    int type;
    int score;
    int depth;
};

typedef uint64_t HashValue;
typedef std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> ZobristTable;
typedef std::unordered_map<HashValue, TranspositionEntry> TranspositionTable;


class Ai
{
private:
    int color;
    int leafnodes;
    int cutoffs;
    std::vector<int> scores;
    ZobristTable zobristtable;
    TranspositionTable tt;

public:
    Ai();
    Ai(int color);
    PieceBoard getMove(Board board);
private:
    PieceBoard randomMove(const Board &board) const;
    void orderMoves(PieceBoardVector &childnodes, const int &color);
    void initZobristTable();
    HashValue getHashValue(const PieceBoard &pb, const int &turn);
    PieceBoard bestMove(const Board &board);
    std::tuple<int,PieceBoard> alphaBetaNegaMax(Board board, int depth, int color, int alpha, int beta);
};