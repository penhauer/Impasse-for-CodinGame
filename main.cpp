#include <iostream>
#include "game.h"
//#include "window.h"

int main()
{
    std::cout << "Welcome to Impasse!\n\nFor the game rules please visit: https://www.marksteeregames.com/Impasse_rules.pdf\n\nEngine and AI implemented by: Balazs Horvath (https://bghorvath.dev)" << std::endl;
    std::cout << "Please select GUI or CLI mode (0 = CLI, 1 = GUI): ";
    int gui, player;
    std::cin >> gui;
    if (gui == 1)
    {
        std::cout << "Unfortunately GUI mode is not yet implemented, defaulting to CLI." << std::endl;
    };
    std::cout << "Choose your color (1 = White, -1 = Black): ";
    std::cin >> player;
    Game game = Game(gui, player);
}