#include "player.h"
#include "game.h"
#include "simple_terminal_player.h"

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


    Game game = Game(timemin);
    game.gameLoop(white, black);


    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Thank you for playing!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    return 0;
}
