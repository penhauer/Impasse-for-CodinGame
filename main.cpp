#include <SFML/Graphics.hpp>
#include "common.h"

using namespace sf;

int main() {
    RenderWindow window(VideoMode(800, 800), "Checkers");
    window.setFramerateLimit(60);

    Board board;
    Piece piece(1, 0, 0);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear();
        board.draw(window);
        piece.draw(window);
        window.display();
    }

    return 0;
}
