#include <ncurses.h>
#include "draw.h"

void draw_head() {
    printw("Supermarket \"Hoof In The Snout\".");
    printw("Queue modeling system");
}

void draw_cashier(int item, Cashier cashier) {
    mvprintw(1, item, "");
    for (int i = 0; i < cashier.queue->length; ++i) {

    }
}
