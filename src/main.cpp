#include "SDL2/SDL.h"
#include "gamestate.h"
#include "ai.h"

int main(int argc, char *argv[])
{
    Game game = Game();
    game.reset();
    game.render_board();
    game.run();
}

class Game
{
private:
    bool pieceSelected = false;
    bool removeSelected = false;
    Piece selectedPiece;
    Pos selectedRemove;
    GameState gamestate;
    SDL_Window *window;
    SDL_Renderer *renderer;
    void select(int x, int y)
    {
        Pos selectedpos = Pos(x / 80, y / 80);
        const int &selected = gamestate.board[selectedpos.col][selectedpos.row];
        // if piece was already selected, try to move it
        if (pieceSelected)
        {
            // if 
            if (((selectedPiece.piece == 2 || selectedPiece.piece == -1) && (selectedpos.row == 7)) || ((selectedPiece.piece == 1 || selectedPiece.piece == -2) && (selectedpos.row == 0)))
            {
                if (removeSelected)
                {
                    gamestate.makeMove(Move(selectedPiece.piece, selectedPiece.pos, selectedpos, selectedRemove));
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
                        gamestate.makeMove(move);
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
    void clean()
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    };
    public:
        void reset()
        {
            GameState gamestate = GameState();
        };
        void render_board()
        {
            SDL_Window *window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if ((i + j) % 2 == 0)
                    {
                        SDL_Rect rect = {i * 40, j * 30, 40, 30};
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }
            };
            SDL_RenderPresent(renderer);
        };
        void run()
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
                    gamestate.makeMove(ai.getMove(gamestate));
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
            clean();
        };
    };