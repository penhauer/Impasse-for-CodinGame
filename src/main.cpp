#include "gamestate.h"
#include "ai.h"
#include "window.h"

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
    Window window;
    void render() {
        window.render_board();
    };
    void reset() {
        gamestate.reset();
        game.render();
    };
    void render() {
        window.clean();
        window.render_board();
    };
    }
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
    public:
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
            window.clean();
        };
    };