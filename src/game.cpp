#include "SDL2/SDL.h"
#include "gamestate.h"
#include "ai.h"

class Game {
public:
    bool pieceSelected = false;
    Game() {
        reset();
        draw();
        run();
    };
    void reset() {
        GameState gameState = GameState();
    };
private:
    void update();
    void render();
    bool running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    void clean() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    };
    void draw() {
        SDL_Window* window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
    void run() {
        SDL_Event event;
        bool running = true;
        while (running) {
            update();
            render();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                };
                if (gameState.getGameState() != 0) {
                    running = false;
                };
                if (gameState.getTurn() == -1) {
                    AI ai = AI();
                    gameState.makeMove(ai.getMove(gameState));
                    } else {
                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            int x, y;
                            SDL_GetMouseState(&x, &y);
                            int col = x / 80;
                            int row = y / 80;
                            if (gameState.getPiece(col, row) != 0) {
                                if (gameState.getPiece(col, row) * gameState.getTurn() > 0) {
                                    pieceSelected = true;
                                    selectedPiece = gameState.getPiece(col, row);
                                };
                            } else if (pieceSelected) {
                                MoveMap moves = gameState.getMoves(selectedPiece);
                                if (moves.find(col+row*8) != moves.end()) {
                                    gameState.makeMove(Move(selectedPiece, col+row*8));
                                    pieceSelected = false;
                                };
                            };
                        };
                    };
                };
        };
        clean();
    };
};
