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
    GameState gamestate;
    SDL_Window *window;
    SDL_Renderer *renderer;
    void select(int x, int y)
    {
        int col = x / 80;
        int row = y / 80;
        if (gamestate.getPiece(col, row) != 0)
        {
            if (gamestate.getPiece(col, row) * gamestate.getTurn() > 0)
            {
                pieceSelected = true;
                selectedPiece = gamestate.getPiece(col, row);
            };
        }
        else if (pieceSelected)
        {
            MoveMap moves = gamestate.getMoves(selectedPiece);
            if (moves.find(col + row * 8) != moves.end())
            {
                gamestate.makeMove(Move(selectedPiece, col + row * 8));
                pieceSelected = false;
            };
        };
    };
    void clean()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
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
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if ((i + j) % 2 == 0) {
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
        Ai ai = Ai();
        while (running)
        {
            if (gamestate.getGameState() != 0)
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
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        select(x, y);
                        if (pieceSelected)
                        {
                            //move piece
                            pieceSelected = false;
                        }
                        else
                        {
                            //select piece
                            pieceSelected = true;
                        }
                        break;
                };
            };
        };
        clean();
    };
};