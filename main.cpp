#include "minimax_player.h"
#include "player.h"
#include "game.h"
#include "random_player.h"
#include "simple_terminal_player.h"
#include <cstdlib>


using namespace std;


void simulateWithTwoPlayers(int games, Player *white, Player *black) {
  int blackWins = 0;
  int whiteWins = 0;
  int totalTurns = 0;
  for (int i = 0; i < games; i++) {
    Game game = Game(white, black);
    int winner = game.gameLoop();
    // std::cout << "winner is: " << (winner == WHITE ? "white" : "black") << " after" << std::endl;
    blackWins += (winner == BLACK) ? 1 : 0;
    whiteWins += (winner == WHITE) ? 1 : 0;
    totalTurns += game.turns;
  }
  cout << "White wins vs blackwins : " << whiteWins << ' ' << blackWins << endl;
  double averageTurns = 1.0 * totalTurns / games;
  cout << "average turns: " << averageTurns << endl;
}

void testRandomAgent() {
  Player *randomPlayer1 = new RandomPlayer(WHITE);
  Player *randomPlayer2 = new RandomPlayer(BLACK);
  simulateWithTwoPlayers(10000, randomPlayer1, randomPlayer2);
}

void testRandomAgentWithSimpleTerminalPlayer() {
  Player *randomPlayer = new RandomPlayer(BLACK);
  Player *user = new SimpleTerminalPlayer(WHITE);
  Game game = Game(user, randomPlayer);
  game.gameLoop();
}


void testRandomAgentWithMiniMaxPlayer() {
  Player *randomPlayer = new RandomPlayer(WHITE);
  Player *miniMaxPlayer = new MiniMaxPlayer(BLACK, 1);
  simulateWithTwoPlayers(10000, randomPlayer, miniMaxPlayer);
}

void testMinimaxAgent() {
  Player *minimaxPlayer1 = new MiniMaxPlayer(WHITE, 3);
  Player *minimaxPlayer2 = new MiniMaxPlayer(BLACK, 5);
  simulateWithTwoPlayers(100, minimaxPlayer1, minimaxPlayer2);
}


int main() {
  srand(time(NULL));
  testRandomAgent();
  // testRandomAgentWithSimpleTerminalPlayer();
  // testRandomAgentWithMiniMaxPlayer();
  // testMinimaxAgent();
  return 0;
}
