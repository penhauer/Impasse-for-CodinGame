#include "gamestate.h"
#include "ai.h"
#include "window.h"

class Game
{
private:
    GameState *gamestate;
    //Ai *ai;
    Window *window;
    void gameLoop()
    {
        while (gamestate->state == 0)
        {
            //if (gamestate->player == 0 || gamestate->player == gamestate->turn)
            //{
            //    ai->doMove();
            //};
            window->render_board();
        };
    };
    Game()
    {
        gamestate = new GameState();
        //ai = new Ai();
        window = new Window();
        gameLoop();
    };
    };
    bool pieceSelected = false;
    bool removeSelected = false;
    Piece selectedPiece;
    int selectedRemove;
    GameState gamestate;
    Window window;
    void reset() {
        window.clean();
        gamestate.reset();
        render();
    };
    void select(int x, int y)
    {
        x = x / 100;
        y = y / 100;
        int pos = x + y * 8;
        const int &selected = gamestate.board.boardarray[pos];
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
            window.clean();
        };
        void render_board()
        {
            window.render_board();
        };
};