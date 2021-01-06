#include <ncurses.h>
#include <stdlib.h>
//#include <stdio.h>
#include "shop_queue_model.h"
#include "draw.h"

static int max_customer_time;
static int max_cashier_queue;
static int max_cashiers;
static int max_next_customers;

static Cashier *g_cashiers = NULL;

void controller();
void model();
void init_settings();

void init_model() {
    init_settings();
    g_cashiers = malloc(max_cashiers * sizeof(Cashier));
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    halfdelay(10);
    draw_head();

    controller();
}

void destroy_model() {
    free(g_cashiers);
    endwin();
}

void controller() {
    const int KEY_ESC = 27;
    int key = getch();
    while (key != KEY_ESC) {
        model();
        key = getch();
    }
}

void model() {

}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
void init_settings() {
    const char *FILE_NAME = "settings.txt";
    FILE *file = fopen(FILE_NAME, "r");
    char word[20];
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_customer_time);
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_cashier_queue);
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_cashiers);
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_next_customers);
    fclose(file);
}
#pragma clang diagnostic pop