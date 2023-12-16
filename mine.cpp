#include "ai_agent.h"
#include "common.h"
#include "game.h"

int main() {
    bool play = true;
    std::string answer;
    int player = 0;
    // ask to choose player and time limit
    player = 0;
    while (player == 0) {
        std::cout << "Choose your color (white/black): ";
        std::cin >> answer;
        if (answer == "white") {
            player = WHITE;
        }
        else if (answer == "black") {
            player = BLACK;
        }
        else {
            std::cout << "Invalid answer" << std::endl;
        }
    }

    int timemin = 3;
    // Player agent = TerminalPlayer();
    // agent.decideOnBoard(Board());


    TerminalPlayer *t = new TerminalPlayer();
    Player *p = t;
    Game game = Game(timemin);
    game.gameLoop(p, NULL);


    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Thank you for playing!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    return 0;
}
