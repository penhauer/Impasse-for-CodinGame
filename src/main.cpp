#include "../include/piece.h"
#include "../include/board.h"
#include <allegro5/allegro.h>

int main() {
    // initialize allegro
    al_init();
    // create display
    ALLEGRO_DISPLAY *display = al_create_display(640, 480);
    // create event queue
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    // register events
    al_register_event_source(event_queue, al_get_display_event_source(display));
    // main loop
    bool done = false;
    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
    }
    // destroy display
    al_destroy_display(display);
    // destroy event queue
    al_destroy_event_queue(event_queue);
    return 0;
}