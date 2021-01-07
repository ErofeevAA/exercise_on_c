#include <ncurses.h>
#include "draw.h"
#include "coords.h"

void draw_cashier(int item, int max_items, Cashier cashier);

void draw_head() {
    printw("Supermarket \"Hoof In The Snout\".");
    printw("Queue modeling system");
}

void draw_all_cashiers(int max_items, int sum_cashiers, Cashier *cashiers) {
    for (int i = 0; i < sum_cashiers; ++i) {
        draw_cashier(i, max_items, cashiers[i]);
    }
}

void draw_time(int time) {
    move(y_time, 0);
    clrtoeol();
    printw("Time of work: %d", time);
    refresh();
}

void draw_next_customers(int num, Customer c1, ...) {
    move(y_next_customers, 0);
    clrtoeol();
    printw("Next customers: ");
    if (num == 0) {
        printw("Nobody");
        refresh();
        return;
    }
    Customer *pointer = &c1;
    for (int i = 0; i < num; ++i) {
        printw("%c%d", pointer->name, pointer->time);
        pointer++;
    }
    refresh();
}

void draw_sum_customers(int sum) {
    move(y_sum_customers, 0);
    clrtoeol();
    printw("Sum of customers in queue: %d", sum);
    refresh();
}

void draw_sum_working_cashiers(int sum, int max) {
    move(y_working_cashiers, 0);
    clrtoeol();
    printw("Working cashiers: %d from %d", sum, max);
    refresh();
}

void draw_sum_served_customers(int sum) {
    move(y_sum_served_customers, 0);
    clrtoeol();
    printw("Sum of served customers: %d", sum);
    refresh();
}

void draw_max_queue_cashier_length(int max) {
    move(y_max_length_queue_cashier, 0);
    //clrtoeol();
    printw("Max length of queue: %d", max);
    refresh();
}

void draw_cashier(int item, int max_items, Cashier cashier) {
    int y_cur = y_cashier;
    int x = (item + 1) * 2 - 1;
    mvprintw(y_cur++, x, "%d", item + 1);
    mvprintw(y_cur++, x, "%d", cashier.sum_served_customers);
    (cashier.is_working) ? mvprintw(y_cur++, x, "+") : mvprintw(y_cur++, x, "-");
    NodeCustomer *tmp = cashier.queue->head;
    for (int i = 0; i < max_items; ++i) {
        for (; tmp != NULL; ++i) {
            mvprintw(y_cur + i, x, "%c%d", tmp->customer.name, tmp->customer.time);
            tmp = tmp->next;
        }
        mvprintw(y_cur + i, x, "||");
    }
    refresh();
}
