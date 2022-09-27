#include "gamestate.h"
#include <iostream>

int main()
{
    GameState gamestate = GameState();
    std::cout << gamestate.board.boardarray[0] << std::endl;

    std::cin.get();
}