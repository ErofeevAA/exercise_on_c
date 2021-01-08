#include <ncurses.h>
#include "draw.h"

int y_cashier = 1;
int y_time;
int y_next_customers;
int y_sum_customers;
int y_working_cashiers;
int y_sum_served_customers;
int y_max_length_queue_cashier;

void init_coords(int max_queue_length) {
    int y_cur = 2 + max_queue_length + y_cashier;
    y_time = y_cur++;
    y_next_customers = y_cur++;
    y_sum_customers = y_cur++;
    y_working_cashiers = y_cur++;
    y_sum_served_customers = y_cur++;
    y_max_length_queue_cashier = y_cur;
}

void draw_cashier(int item, int max_items, Cashier cashier);

void draw_head() {
    printw("Supermarket \"Hoof In The Snout\". ");
    printw("Queue modeling system.");
    refresh();
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

void draw_next_customers(int length, Customer *customers) {
    move(y_next_customers, 0);
    clrtoeol();
    printw("Next customers:");
    if (length == 0) {
        printw(" Nobody");
        refresh();
        return;
    }
    for (int i = 0; i < length; ++i) {
        printw(" %c%d", customers[i].name, customers[i].time);
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
    int x = (item + 1) * 6 - 5;
    mvprintw(y_cur++, x + 1, "%d", item + 1);
    (cashier.is_working) ? mvprintw(y_cur++, x + 1, "+") : mvprintw(y_cur++, x + 1, "-");
    NodeCustomer *tmp = cashier.queue->head;
    for (int i = 0; i < max_items; ++i) {
        for (; tmp != NULL; ++i) {
            mvprintw(y_cur + i, x, "%c %d", tmp->customer.name, tmp->customer.time);
            tmp = tmp->next;
        }
        mvprintw(y_cur + i, x, "| |");
    }
    refresh();
}