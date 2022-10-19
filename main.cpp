#include <iostream>
#include "game.h"
#include <thread>
#include <chrono>

int main()
{
    std::cout << "Welcome to Impasse!\n\nFor the game rules write 'rules' or visit: https://www.marksteeregames.com/Impasse_rules.pdf\n\nEngine and AI implemented by: Balazs Horvath (https://bghorvath.dev)" << std::endl;
    bool play = true;
    while (play)
    {
        std::string answer;
        int player = 0;
        while (player == 0)
        {
            std::cout << "Choose your color (white/black): ";
            std::cin >> answer;
            if (answer == "white")
            {
                player = 1;
            }
            else if (answer == "black")
            {
                player = -1;
            }
            else
            {
                std::cout << "Invalid answer" << std::endl;
            };
        };
        Game game = Game(player, 5);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "Would you like to play again? (yes/no): ";
        std::cin >> answer;
        play = answer == "yes";
    };
    std::cout << "Thank you for playing!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    return 0;
}