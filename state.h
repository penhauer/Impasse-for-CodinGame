#pragma  once

#include "board.h"
#include <vector>

class State {
  public:
    int turn;

    std::vector<PieceBoard>  possiblepieceboards;
    std::vector<PieceBoard>  pieceboardhistory;
    PieceBoard pieceboard;

    State();
    State(int turn, PieceBoard *pb);

    void printBoard();
    void doMove(int moveNumber);
    void doMove(PieceBoard nextPieceBoard);
    void undoMove();

    State newChildState();



  private:
    std::vector<Pos> singles;

    void generateLegalMoves();



    bool singlesGenerated = false;
    void findAllSingles(int color);

    void addPieceMoves(Pos pos);
    void checkMove(Pos pos, Pos toPos);
    void addImpassable();
    void changeTurn();

    bool isTransposable(Pos pos, Pos toPos);
    void checkImpasseForPos(Pos pos);

    void checkBearOff(Pos pos, Pos toPos);
    void checkCrown(Pos pos, Pos toPos);
    void doSimpleMove(Pos pos, Pos toPos);

    bool inside(Pos pos);
};
