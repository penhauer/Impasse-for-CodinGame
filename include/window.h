#include "SDL2/SDL.h"
#include <iostream>

SDL_Window *window;
SDL_Renderer *renderer;

class Window
{
public:
    Window();
    ~Window();
    void render_board();
    void clean();
};