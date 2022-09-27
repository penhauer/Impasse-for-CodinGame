#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "board.h"
#include "ai.h"

class Game
{
private:
    int player; // 1 = White vs Ai, -1 = Black vs Ai
    Board board;
    Ai ai;
    Game()
    {
        board = Board();
        std::cout << "Choose your color (white/black): ";
        std::string playercolor;
        std::cin >> playercolor;
        player = (playercolor == "white") ? 1 : -1;
        ai = Ai();
        gameLoop();
    };
    void gameLoop()
    {
        Move move;
        while (board.state == 0)
        {
            if (player == board.turn)
            {
                std::cout << "Your turn" << std::endl;
                bool turnEnd = false;
                while (turnEnd)
                {
                    std::cout << "Select position: ";
                    std::string notation;
                    std::cin >> notation;
                    int pos = notationToIndex(notation);
                    turnEnd = trySelect(pos);
                };
                board.doMove(move);
            }
            else
            {
                std::cout << "Ai turn" << std::endl;
                Move move = ai.getMove(board);
                board.doMove(move);
            };
            board.print();
        };
    };
    bool trySelect(int pos)
    {
        if (board.movemap.count(pos) == 0)
        {
            std::cout << "Invalid position" << std::endl;
            return false;
        }
        else
        {

            const MoveSet &moveset = board.movemap[pos];
            if (moveset.size() == 1)
            {
                Move move = Move(pos, moveset.begin);
                return true;
            }
            else
            {
                std::cout << "Select move: ";
                std::string notation;
                std::cin >> notation;
                int move = notationToIndex(notation);
                if (moveset.count(move) == 0)
                {
                    std::cout << "Invalid move" << std::endl;
                    return false;
                }
                else
                {
                    move = Move(pos, move);
                    return true;
                };
            };
        }
        if (
        {
            
            return true;
        }
        else
        {
            return false;
        };
    };
    bool pieceSelected = false;
    bool removeSelected = false;
    Piece selectedPiece;
    int selectedRemove;
    Board board;
    void reset() {
        board.reset_board();
    };
    //concert from chess notation to position
    int notationToIndex(const std::string &notation)
    {
        char letter = tolower(notation[0]);
        int row = notation[1] - '1';
        int col = (int)letter-97;
        row = (int)row;
        return col + 8 * row;
    };
    void select(int pos)
    {
        const int &selected = board.boardarray[pos];
        // if piece was already selected, try to move it
        if (pieceSelected)
        {
            // if 
            if (((selectedPiece.piece == 2 || selectedPiece.piece == -1) && (selectedpos.row == 7)) || ((selectedPiece.piece == 1 || selectedPiece.piece == -2) && (selectedpos.row == 0)))
            {
                if (removeSelected)
                {
                    gamestate.doMove(Move(selectedPiece.piece, selectedPiece.pos, selectedpos, selectedRemove));
                    pieceSelected = false;
                    removeSelected = false;
                    return;
                }
                else
                {
                    if (gamestate.movemap.count(selected) > 0)
                    {

                    }
                    selectedRemove = selectedpos;
                    removeSelected = true;
                    return;
                };
            }
            else
            {
                Move move = Move(selectedPiece.piece, selectedPiece.pos, selectedpos);
                if (gamestate.movemap.count(selectedPiece.pos) > 0)
                {
                    MoveSet moves = gamestate.movemap[selectedPiece.pos];
                    if (moves.count(selectedpos) > 0)
                    {
                        gamestate.doMove(move);
                        pieceSelected = false;
                        return;
                    }
                    else
                    {
                        pieceSelected = false;
                        return;
                    }
                    ;
                };
            };
        }
        // else select a piece first, then recursively call select again
        else
        {
            if (gamestate.movemap.count(selectedpos) > 0)
            {
                pieceSelected = true;
                selectedPiece = Piece(piece, selectedpos);
                return;
            }
            else
            {
                pieceSelected = false;
                return;
            };
        };
    };
    public:
        void gameLoop()
        {
            bool running = true;
            SDL_Event event;
            Ai ai = Ai(-1);
            while (running)
            {
                if (gamestate.state != 0)
                {
                    running = false;
                };
                if (gamestate.getTurn() == -1)
                {
                    gamestate.doMove(ai.getMove(gamestate));
                };
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                    case SDL_QUIT:
                        running = false;
                    case SDL_MOUSEBUTTONDOWN:
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        select(x, y);
                    };
                };
            };
        };