#include "../board.h"
#include "../game.h"

struct Scenario
{
    int turn;
    PieceToCrown piecetocrown;
    PieceSet pieceset;
    Move move;
};

std::vector<Scenario> getScenarios()
{
    std::vector<Scenario> scenarios;
    Scenario scenario1;
    scenario1.turn = 1;
    scenario1.piecetocrown = PieceToCrown({{1, Piece(1, 1, 7, 0)}});
    scenario1.pieceset = PieceSet({Piece(1, 1, 7, 0), Piece(2, 1, 1, 0), Piece(1, -1, 5, 5)});
    scenario1.move = Move(Piece(2, 1, 1, 0), Piece(0, 0, 0, 1), Piece(0, 0, -1, -1));
    scenarios.push_back(scenario1);
    return scenarios;
};

int main()
{
    Game game = Game();
    std::vector<Scenario> scenarios = getScenarios();
    for (Scenario scenario : scenarios)
    {
        game.board.loadBoard(scenario.turn, scenario.piecetocrown, scenario.pieceset);
        game.board.doMove(scenario.move);
        game.board.printBoard();
    };
    return 0;
}