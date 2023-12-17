#include "greedy_player.h"
#include "player.h"
#include "game.h"
#include "random_player.h"
#include "simple_terminal_player.h"
#include <cstdlib>


using namespace std;



void testRandomAgent() {
  srand(time(NULL));
  Player *randomPlayer1 = new RandomPlayer(WHITE);
  Player *randomPlayer2 = new RandomPlayer(BLACK);
  int blackWins = 0;
  int whiteWins = 0;
  int totalTurns = 0;
  int n = 10000;
  for (int i = 0; i < n; i++) {
    Game game = Game(randomPlayer1, randomPlayer2);
    int winner = game.gameLoop();
    // std::cout << "winner is: " << (winner == WHITE ? "white" : "black") << " after" << std::endl;

    blackWins += (winner == BLACK) ? 1 : 0;
    whiteWins += (winner == WHITE) ? 1 : 0;
    totalTurns += game.turns;
  }
  cout << "White wins vs blackwins : " << whiteWins << ' ' << blackWins << endl;
  double averageTurns = 1.0 * totalTurns / n;
  cout << "average turns: " << averageTurns << endl;
}

void testRandomAgentWithSimpleTerminalPlayer() {
  srand(time(NULL));
  Player *randomPlayer = new RandomPlayer(BLACK);
  Player *user = new SimpleTerminalPlayer(WHITE);
  Game game = Game(user, randomPlayer);
  game.gameLoop();
}


void testRandomAgentWithGreedyAgent() {
  srand(time(NULL));
  Player *randomPlayer1 = new RandomPlayer(WHITE);
  Player *greedyPlayer = new GreedyPlayer(BLACK);
  int blackWins = 0;
  int whiteWins = 0;
  int totalTurns = 0;
  int n = 10000;
  for (int i = 0; i < n; i++) {
    Game game = Game(randomPlayer1, greedyPlayer);
    int winner = game.gameLoop();
    // std::cout << "winner is: " << (winner == WHITE ? "white" : "black") << " after" << std::endl;
    blackWins += (winner == BLACK) ? 1 : 0;
    whiteWins += (winner == WHITE) ? 1 : 0;
    totalTurns += game.turns;
  }
  cout << "White wins vs blackwins : " << whiteWins << ' ' << blackWins << endl;
  double averageTurns = 1.0 * totalTurns / n;
  cout << "average turns: " << averageTurns << endl;
}

int main() {
  // testRandomAgent();
  // testRandomAgentWithSimpleTerminalPlayer();
  testRandomAgentWithGreedyAgent();
  return 0;
}
