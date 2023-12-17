#include "player.h"
#include "game.h"
#include "random_player.h"
#include "simple_terminal_player.h"
#include <cstdlib>


using namespace std;

int main() {
    // bool play = true;
    // std::string answer;
    // ask to choose player and time limit
    // int player = 0;
    // player = 0;
    // while (player == 0) {
    //     std::cout << "Choose your color (white/black): ";
    //     std::cin >> answer;
    //     if (answer == "white") {
    //         player = WHITE;
    //     } else if (answer == "black") {
    //         player = BLACK;
    //     } else {
    //         std::cout << "Invalid answer" << std::endl;
    //     }
    // }

    int timemin = 3;
    // Player agent = TerminalPlayer();
    // agent.decideOnBoard(Board());


    Player *white = new SimpleTerminalPlayer();


    auto timer = std::make_tuple(timemin * 60 * 1000, timemin * 60 * 1000);
    Player *black = new Ai(WHITE * -1, std::get<1>(timer));


    srand(time(NULL));

    Player *randomPlayer = new RandomPlayer();


    int blackWins = 0;
    int whiteWins = 0;
    for (int i = 0; i < 1000; i++) {
      Game game = Game();
      int winner = game.gameLoop(randomPlayer, randomPlayer);
      std::cout << "winner is: " << (winner == WHITE ? "white" : "black") << " after" << std::endl;

      blackWins += (winner == BLACK) ? 1 : 0;
      whiteWins += (winner == WHITE) ? 1 : 0;
    }

    cout << "White wins vs blackwins : " << whiteWins << ' ' << blackWins << endl;



    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Thank you for playing!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    return 0;
}
