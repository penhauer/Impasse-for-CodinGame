#include "ai_agent.h"



// void Player::decideOnBoard(Board board) {
//     std::cout << "BAAAAAAAAAAAAAAAAAdDDDDDDDD" << std::endl;
// }

void AIPlayer::decideOnBoard(Board &board) {
    board.generateLegalMoves();
    board.printBoard();
    std::cout << "hendel" << std::endl;
};

