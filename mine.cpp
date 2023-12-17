#include "player.h"
#include "game.h"
#include "random_player.h"
#include "simple_terminal_player.h"
#include <cstdlib>


using namespace std;



void testRandomAgent() {
  srand(time(NULL));
  Player *randomPlayer = new RandomPlayer();
  int blackWins = 0;
  int whiteWins = 0;
  int totalTurns = 0;
  int n = 10000;
  for (int i = 0; i < n; i++) {
    Game game = Game(randomPlayer, randomPlayer);
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
  Player *randomPlayer = new RandomPlayer();
  Player *user = new SimpleTerminalPlayer();
  Game game = Game(user, randomPlayer);
  game.gameLoop();
}

int main() {
  int timemin = 3;
  Player *white = new SimpleTerminalPlayer();
  auto timer = std::make_tuple(timemin * 60 * 1000, timemin * 60 * 1000);
  Player *black = new Ai(WHITE * -1, std::get<1>(timer));

  testRandomAgent();
  // testRandomAgentWithSimpleTerminalPlayer();

  return 0;
}
