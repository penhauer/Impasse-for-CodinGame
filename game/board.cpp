#include "common.h"

class Board
    {
        private:
            int turn; // 1 = white, -1 = black
            int gameState; // 0 = in progress, 1 = white win, -1 = black win
            int whiteSingles; //number of white singles
            int whiteDoubles; //number of white doubles
            int blackSingles; //number of black singles
            int blackDoubles; //number of black doubles
    };
    {
        public:
            Board
            {
                turn = 1;
                gameState = 0;
                whiteSingles = 6;
                whiteDoubles = 6;
                blackSingles = 6;
                blackDoubles = 6;
            }
            void selectPiece(int x, int y) {
                selectedPiece = board[x][y];
                if (turn == selectedPiece.color) {
                    bool pieceSelected = true;
                }
            }
            void movePiece(int x, int y, int newX, int newY) {
                board[newX][newY] = board[x][y];
                board[x][y] = 0;
                turn = turn * -1;
            }

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
            }
    };