#include <ncurses.h>
#include <stdlib.h>
#include "shop_queue_model.h"
#include "draw.h"
#include "operation_shop_struct.h"
#include "coords.h"

static int max_customer_time;
static int max_cashier_queue_length;
static int max_cashiers;
static int max_next_customers;

static Cashier *g_cashiers = NULL;

static int g_time = 0;
static int g_served_customers = 0;

void controller();
void model();
void init_settings();
void init_cashiers();

void init_model() {
    init_settings();
    init_cashiers();
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    halfdelay(10);
    draw_head();
    draw_all_cashiers(max_cashier_queue_length, max_cashiers, g_cashiers);
    draw_time(g_time);
    //draw_next_customers();
    //draw_sum_customers(0);
    //draw_sum_working_cashiers();
    draw_sum_served_customers(g_served_customers);
    draw_max_queue_cashier_length(max_cashier_queue_length);
    controller();
}

void destroy_model() {
    free_all_cashiers_queue(&g_cashiers, max_cashiers);
    free(g_cashiers);
    endwin();
}

void controller() {
    const int KEY_ESC = 27;
    int key = getch();
    while (key != KEY_ESC) {
        g_time++;
        model();
        key = getch();
    }
}

void model() {
    draw_time(g_time);
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
    fscanf(file, "%d", &max_cashier_queue_length);
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_cashiers);
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_next_customers);
    fclose(file);
    int y_cur = 3 + max_cashier_queue_length + y_cashier;
    y_time = y_cur++;
    y_next_customers = y_cur++;
    y_sum_customers = y_cur++;
    y_working_cashiers = y_cur++;
    y_sum_served_customers = y_cur++;
    y_max_length_queue_cashier = y_cur;
}
#pragma clang diagnostic pop

void init_cashiers() {
    g_cashiers = malloc(max_cashiers * sizeof(Cashier));
    for (int i = 0; i < max_cashiers; ++i) {
        g_cashiers[i].queue = NULL;
    }
}