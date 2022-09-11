#include "common.h"

void Board() {
    // init board with grid for the Pieces
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(i < 3 && (i+j)%2 == 1)
                board[i][j] = new int Piece(1, i, j);
            else if(i > 4 && (i+j)%2 == 1)
                board[i][j] = new int Piece(2, i, j);
            else
                board[i][j] = 0;
        }
    };
}