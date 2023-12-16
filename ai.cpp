#include "ai.h"
#include "board.h"
#include "common.h"
#include <cassert>
#include <vector>

Ai::Ai(int color, int AITime) : color(color), AITime(AITime) { 
  initZobristTable(); 
}

int Ai::decideOnBoard(Board board) {
  auto pieceboard = getMove(board);
  int ind = -1;
  for (int i = 0; i < board.possiblepieceboards.size(); i++) {
    auto x = board.possiblepieceboards[i];
    if (x.lastmove == pieceboard.lastmove) {
      assert(ind == -1);
      ind = i;
    }
  }
  return ind;
}


// Use iterative deepening to fill transposition table and find the best move by calling alpha-beta search.
// Use dynamic time allocation based on the time left and expected number of moves left.
PieceBoard Ai::getMove(Board board)
{
    cutoffs.clear();
    leafnodes = 0;
    int searchdepth = 1;
    int score;
    PieceBoard pieceboard;
    int elapsedplys = board.pieceboardhistory.size();
    const int expectedmaxplys = 100;
    int expectedmovesleft = std::max(10, (expectedmaxplys - elapsedplys) / 2); // expected moves left for AI: always assume at least 10 moves left
    int timelimit = std::min(20000, AITime / expectedmovesleft);               // minimum of 20sec and remaining time / expected moves left for AI
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    end = std::chrono::system_clock::now();
    while (end - start < std::chrono::milliseconds(timelimit))
    {
        searchdepth += 1;
        std::tie(score, pieceboard) = alphaBetaNegaMax(board, searchdepth, color, -100000, 100000);
        end = std::chrono::system_clock::now();
    };
    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
    // Approximate pruned nodes based on average branching factor and cutoffs / depth
    int cutoffsum = std::accumulate(cutoffs.begin(), cutoffs.end(), 0, [](int sum, const std::pair<int, int> &p) { return sum + p.second; });
    const double avgbranchingfactor = 15.3; // calculated beforehand and hardcoded since not worth the computation time
    double multiplier = 1.0;
    int prunedleafnodes = 0;
    for (auto iter = cutoffs.rbegin(); iter != cutoffs.rend(); ++iter)
    {
        prunedleafnodes += (double)iter->second * multiplier;
        multiplier *= avgbranchingfactor;
    };
    std::cout << "Search with final depth of " << searchdepth << " returned best move (with score " << score << ") after " << duration << "s.\nIt evaluated " << leafnodes << " leaf nodes, and made " << cutoffsum << " cutoffs (pruning around ~" << prunedleafnodes << ")." << std::endl;
    return pieceboard;
};
// Recursively execute Negamax search algorithm using alpha-beta pruning and transposition tables
std::tuple<int, PieceBoard> Ai::alphaBetaNegaMax(Board board, int depth, int color, int alpha, int beta)
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
            return std::make_tuple(tt.at(hash).score, board.pieceboard);
        };
    };
    if (depth == 0 || board.winner != BOARD_GAME_ONGOING)
    {
        leafnodes += 1;
        return std::make_tuple(board.pieceboard.evaluate(color), board.pieceboard);
    };
    int bestscore = -10000;
    PieceBoard bestpieceboard;
    board.generateLegalMoves();
    std::vector<PieceBoard> childnodes = board.possiblepieceboards;
    orderMoves(childnodes, color);
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
            addCutoff(depth);
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
void Ai::addCutoff(const int &depth)
{
    if (cutoffs.count(depth) > 0)
    {
        cutoffs[depth] += 1;
    }
    else
    {
        cutoffs[depth] = 1;
    };
};
// Order moves from best to worst based on transposition table entries and heuristic evaluation
void Ai::orderMoves(std::vector<PieceBoard> &childnodes, const int &color)
{
    std::map<Move, int> nodescores;
    for (PieceBoard child : childnodes)
    {
        HashValue hash = getHashValue(child, color);
        int value = 0;
        if (tt.count(hash) > 0)
        {
            value = tt.at(hash).score;
        }
        else
        {
            value = child.evaluate(color);
        };
        nodescores[child.lastmove] = value;
    };
    std::sort(begin(childnodes), end(childnodes), [&](PieceBoard &a, PieceBoard &b)
              { return nodescores[a.lastmove] > nodescores[b.lastmove]; });
};
// Create a hash value for the given board using the random numbers from the Zobrist table
HashValue Ai::getHashValue(PieceBoard &pb, const int &turn)
{
    // generate zobrist key from pb and turn
    HashValue value = 0;
    for (int i = 0; i < 64; i++)
    {
      int row = i / COLS;
      int col = i % COLS;
      Pos pos = Pos(row, col);

        if (!pb.isEmpty(pos))
        {
            Piece p = pb.getPiece(pos);
            int piece;
            switch (p.color)
            {
            case WHITE:
                switch (p.pieceCount)
                {
                case SINGLE_COUNT:
                    piece = 1;
                    break;
                case DOUBLE_COUNT:
                    piece = 2;
                    break;
                };
                break;
            case BLACK:
                switch (p.pieceCount)
                {
                case SINGLE_COUNT:
                    piece = 3;
                    break;
                case DOUBLE_COUNT:
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
// Iint ZobristTable for hashing
void Ai::initZobristTable()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<HashValue> dist(0, std::numeric_limits<HashValue>::max());
    for (int i = -1; i < 2; i+=2)
    {
        for (int j = 0; j < 64; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                zobristtable[i][j][k] = dist(mt);
            };
        };
    };
};
