#include "SDL2/SDL.h"
#include <iostream>

class Window
{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Window();
    ~Window();
    void render_board();
    void clean();
};