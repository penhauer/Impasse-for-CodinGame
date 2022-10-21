#include "board.h"
#include "ai.h"
#include <algorithm>
#include <set>
#include <iostream>
#include <vector>

Ai::Ai(){};
Ai::Ai(int color) : color(color) { initZobristTable(); };
PieceBoard Ai::getMove(Board board, const int &aitime)
{
    scores.clear();
    cutoffs = 0;
    leafnodes = 0;
    int searchdepth = 1;
    int score;
    PieceBoard pieceboard;
    int elapsedplys = board.pieceboardhistory.size();
    int expectedmaxplys = 100;
    int expectedmovesleft = std::max(10,(expectedmaxplys-elapsedplys)/2); // expected moves left for AI: always assume at least 10 moves left
    int timelimit = std::min(20000, aitime / expectedmovesleft); // minimum of 20sec and remaining time / expected moves left for AI
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    end = std::chrono::system_clock::now();
    while (end - start < std::chrono::milliseconds(timelimit))
    {
        searchdepth += 1;
        std::tie(score, pieceboard) = alphaBetaNegaMax(board, searchdepth, color, -100000, 100000);
        end = std::chrono::system_clock::now();
    };
    int maxIndex = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()/1000.0;
    std::cout << searchdepth << " ply deep search returned best move (with score " << score << ") after " << duration << "s.\nIt considered " << leafnodes << " leaf nodes, and made " << cutoffs << " cutoffs." << std::endl;
    return pieceboard;
};
std::tuple<int,PieceBoard> Ai::alphaBetaNegaMax(Board board, int depth, int color, int alpha, int beta)
{
    HashValue hash = getHashValue(board.pieceboard, color);
    int oldalpha = alpha;
    if (tt.count(hash) > 0 && tt.at(hash).depth >= depth)
    {
        if (tt.at(hash).type == 0) // exact score
        {
            return std::make_tuple(tt.at(hash).score, board.pieceboard);
        }
        else if (tt.at(hash).type == 1) // lower bound
        {
            alpha = std::max(alpha, tt.at(hash).score);
        }
        else if (tt.at(hash).type == 2) // upper bound
        {
            beta = std::min(beta, tt.at(hash).score);
        };
        if (alpha >= beta)
        {
            cutoffs++;
            return std::make_tuple(tt.at(hash).score, board.pieceboard);
        };
    };
    if (depth == 0 || board.state != 0)
    {
        leafnodes += 1;
        return std::make_tuple(board.pieceboard.evaluate(color), board.pieceboard);
    };
    int bestscore = -10000;
    PieceBoard bestpieceboard;
    board.createPossibleBoards();
    PieceBoardVector childnodes = board.possiblepieceboards;
    //orderMoves(childnodes, color);
    for (PieceBoard child : childnodes)
    {
        board.doMove(child);
        int score;
        PieceBoard pb;
        std::tie(score, pb) = alphaBetaNegaMax(board, depth - 1, -color, -beta, -alpha);
        score = -score;
        if (score > bestscore)
        {
            bestscore = score;
            bestpieceboard = child;
        };
        if (bestscore > alpha)
        {
            alpha = bestscore;
        };
        if (bestscore >= beta)
        {
            cutoffs += 1;
            break;
        };
        board.undoMove();
    };
    if (bestscore <= oldalpha)
    {
        tt[hash] = {0, bestscore, depth};
    }
    else if (bestscore >= beta)
    {
        tt[hash] = {1, bestscore, depth};
    }
    else
    {
        tt[hash] = {2, bestscore, depth};
    };
    return std::make_tuple(bestscore, bestpieceboard);
};
PieceBoard Ai::randomMove(const Board &board) const
{
    const PieceBoardVector &moves = board.possiblepieceboards;
    int random = rand() % moves.size();
    return moves[random];
};
void Ai::orderMoves(PieceBoardVector &childnodes, const int &color)
{
    std::map<Move, int> nodescores;
    for (PieceBoard child : childnodes)
    {
        HashValue hash = getHashValue(child, color);
        int value = 0;
        if (tt.count(hash) > 0)
        {
            value = tt[hash].score;
        }
        else
        {
            value = child.evaluate(color);
        };
        nodescores[child.lastmove] = value;
    };
    std::sort(begin(childnodes), end(childnodes), [&](PieceBoard &a, PieceBoard &b) { return nodescores[a.lastmove] > nodescores[b.lastmove]; });
};
HashValue Ai::getHashValue(const PieceBoard &pb, const int &turn)
{
    //generate zobrist key from pb and turn
    HashValue value = 0;
    for (int i = 0; i < 64; i++)
    {
        if (pb.piecemap.count(i) > 0)
        {
            Piece p = pb.piecemap.at(i);
            int piece;
            switch (p.color)
            {
                case 1:
                    switch (p.piece)
                    {
                    case 1:
                        piece = 1;
                        break;
                    case 2:
                        piece = 2;
                        break;
                    };
                    break;
                case -1:
                    switch (p.piece)
                    {
                    case 1:
                        piece = 3;
                        break;
                    case 2:
                        piece = 4;
                        break;
                    };
                    break;
            };
            value ^= zobristtable.at(turn).at(i).at(piece);
        }
        else
        {
            value ^= zobristtable.at(turn).at(i).at(0);
        };
    };
    return value;
};
// initialize ZobristTable
void Ai::initZobristTable()
{
    for (int i = -1; i < 2; i+=2)
    {
        for (int j = 0; j < 64; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                zobristtable[i][j][k] = rand();
            };
        };
    };
};
