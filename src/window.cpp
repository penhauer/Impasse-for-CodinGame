
#include "SDL2/SDL.h"
#include <iostream>

SDL_Window *window;
SDL_Renderer *renderer;

class Window
{
public:
    Window()
    {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
        if (window) {
			std::cout << "Window Created!\n";
        };
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
			SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
			std::cout << "Renderer created!\n";
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        };
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
    void clean()
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    };
};