
#include <iostream>
#include "window.h"
#include "board.h"

class Window
{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Window()
    {
        //Init SDL
        SDL_Init(SDL_INIT_EVERYTHING);
        //Init Window
        window = SDL_CreateWindow("Impasse", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
        //Init Renderer
        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    };
    ~Window()
    {
        clean();
    };
    void render_board()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if ((i + j) % 2 == 0)
                {
                    SDL_Rect rect = {i * 100, j * 100, 100, 100};
                    SDL_RenderFillRect(renderer, &rect);
                };
            };
        };
        SDL_RenderPresent(renderer);
    };
    // Draw the array of pieces as circles on the window using the renderer
    void render_piece(BoardArray boardarray)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (boardarray[i][j] != 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_Rect rect = {i * 100 + 25, j * 100 + 25, 50, 50};
                    SDL_RenderFillRect(renderer, &rect);
                    if (boardarray[i][j] == 1)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    }
                    else if (boardarray[i][j] == 2)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    }
                    else if (boardarray[i][j] == -1)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    }
                    else if (boardarray[i][j] == -2)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    }
                    SDL_Rect rect2 = {i * 100 + 30, j * 100 + 30, 40, 40};
                    SDL_RenderFillRect(renderer, &rect2);
                };
            };
        };
        SDL_RenderPresent(renderer);
    };
    };
    };
    void clean()
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    };
};