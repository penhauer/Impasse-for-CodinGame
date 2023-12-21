#include "minimax_player.h"
#include "player.h"
#include "game.h"
#include "random_player.h"
#include "simple_terminal_player.h"
#include "mcts_player.h"
#include <cstdlib>


using namespace std;


void simulateWithTwoPlayers(int games, Player *white, Player *black) {
  int blackWins = 0;
  int whiteWins = 0;
  int totalTurns = 0;
  for (int i = 0; i < games; i++) {
    Game game = Game();
    int winner = game.gameLoop(white, black);
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
  simulateWithTwoPlayers(500, randomPlayer1, randomPlayer2);
}

void testRandomAgentWithSimpleTerminalPlayer() {
  Player *user = new SimpleTerminalPlayer(WHITE);
  Player *randomPlayer = new RandomPlayer(BLACK);
  simulateWithTwoPlayers(1, user, randomPlayer);
}


void testRandomAgentWithMiniMaxPlayer() {
  Player *randomPlayer = new RandomPlayer(WHITE);
  Player *miniMaxPlayer = new MiniMaxPlayer(BLACK, 1);
  simulateWithTwoPlayers(10000, randomPlayer, miniMaxPlayer);
}

void testMinimaxAgent() {
  Player *minimaxPlayer1 = new MiniMaxPlayer(WHITE, 2);
  Player *minimaxPlayer2 = new MiniMaxPlayer(BLACK, 1);
  simulateWithTwoPlayers(1, minimaxPlayer1, minimaxPlayer2);
}


void playInCodinGame() {
    string colorStr; // color of your pieces ("w" or "b")
    cin >> colorStr; cin.ignore();
    int color = colorStr == "w" ? WHITE : BLACK;

    Player *us = new MiniMaxPlayer(color, 3);
    // State state = State();
    // Player *us = new MCTSPlayer(color, &state);



    Game game = Game();
    game.codingGameLoop(us);
}

void testMCTSPlayer() {
  State state = State();
  Player *mcts_player = new MCTSPlayer(WHITE, &state);
  Player *randomPlayer = new RandomPlayer(BLACK);
  Player *miniMaxPlayer = new MiniMaxPlayer(BLACK, 6);
  // Player *simpleTerminalPlayer = new SimpleTerminalPlayer(BLACK);
  Player *p1 = mcts_player;
  Player *p2 = miniMaxPlayer;

  int blackWins = 0;
  int whiteWins = 0;
  int totalTurns = 0;
  int games = 1;
  for (int i = 0; i < games; i++) {
    Game game = Game(&state);
    game.print = true;
    int winner = game.gameLoop(p1, p2);
    // std::cout << "done with the game " << i + 1 << std::endl;
    blackWins += (winner == BLACK) ? 1 : 0;
    whiteWins += (winner == WHITE) ? 1 : 0;
    totalTurns += game.turns;
  }
  cout << "White wins vs blackwins : " << whiteWins << ' ' << blackWins << endl;
  double averageTurns = 1.0 * totalTurns / games;
  cout << "average turns: " << averageTurns << endl;
}

int main() {
  srand(time(NULL));
  // testRandomAgent();
  // testRandomAgentWithSimpleTerminalPlayer();
  // testRandomAgentWithMiniMaxPlayer();
  // testMinimaxAgent();
  playInCodinGame();
  // testMCTSPlayer();
  return 0;
}
