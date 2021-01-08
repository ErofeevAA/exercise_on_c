#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "shop_queue_model.h"
#include "draw.h"
#include "operation_shop_struct.h"

static int max_customer_time;
static int max_queue_length;
static int max_sum_cashiers;
static int max_next_customers;

static Cashier *g_cashiers = NULL;

static int g_time = 0;

static Customer *g_next_customers = NULL;
static int g_length_next_customers = 0;

void controller();

void model();

void push_customers();
void pop_customers();

void init_settings();
void init_cashiers();

void regenerate_customers();

int sum_customers_in_queue();
int sum_served_customers();
int sum_working_cashier();

bool is_overflow();

void init_model() {
    init_settings();
    init_coords(max_queue_length);
    init_cashiers();
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    // halfdelay(n) меняет функцию getch()
    // после вызова первой, вторая ждёт n/10 секунд и
    // если пользователь не проявил активность
    // возвращает значение ошибки
    halfdelay(10);
    draw_head();
    model();
    draw_max_queue_cashier_length(max_queue_length);
    controller();
}

void destroy_model() {
    // отключает halfdelay() режим
    cbreak();
    draw_stop();
    getch();
    free_all_cashiers_queue(&g_cashiers, max_sum_cashiers);
    free(g_cashiers);
    free(g_next_customers);
    endwin();
}

void controller() {
    const int KEY_ESC = 27;
    int key = getch();
    while (key != KEY_ESC) {
        g_time++;
        model();
        if (is_overflow()) {
            break;
        }
        key = getch();
    }
}

void model() {
    pop_customers();
    if (g_next_customers != NULL) {
        push_customers();
    }
    srand(time(0));
    g_length_next_customers = rand() % (max_next_customers + 1);
    regenerate_customers();
    draw_all_cashiers(max_queue_length, max_sum_cashiers, g_cashiers);
    draw_time(g_time);
    draw_next_customers(g_length_next_customers, g_next_customers);
    draw_sum_customers(sum_customers_in_queue());
    draw_sum_working_cashiers(sum_working_cashier(), max_sum_cashiers);
    draw_sum_served_customers(sum_served_customers());
}

void push_customers() {
    int l = 0;
    Cashier *ptr = g_cashiers;
    for (int i = 0; i < max_sum_cashiers; ++i) {
        for (int j = ptr->queue->length; l < g_length_next_customers && j < max_queue_length; ++j) {
            push_customer(ptr, g_next_customers[l]);
            l++;
        }
        if (l == g_length_next_customers) {
            break;
        }
        ptr++;
    }
}

void pop_customers() {
    Cashier *ptr = g_cashiers;
    for (int i = 0; i < max_sum_cashiers; ++i) {
        if (ptr->queue->head == NULL) {
            ptr++;
            continue;
        }
        (ptr->queue->head->customer.time)--;
        if (ptr->queue->head->customer.time == 0) {
            pop_customer(ptr);
        }
        ptr++;
    }
}

//без этих макросов CLion пристаёт, что fscanf не выбрасывает сообщение об ошибке
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
    fscanf(file, "%d", &max_queue_length);
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_sum_cashiers);
    fscanf(file, "%s", word);
    fscanf(file, "%s", word);
    fscanf(file, "%d", &max_next_customers);
    fclose(file);
}
#pragma clang diagnostic pop

void init_cashiers() {
    g_cashiers = malloc(max_sum_cashiers * sizeof(Cashier));
    for (int i = 0; i < max_sum_cashiers; ++i) {
        g_cashiers[i].queue = malloc(sizeof(QueueCustomer));
        g_cashiers[i].queue->head = NULL;
        g_cashiers[i].queue->length = 0;
        g_cashiers[i].sum_served_customers = 0;
        g_cashiers[i].is_working = false;
    }
}

void regenerate_customers() {
    if (g_length_next_customers == 0) {
        g_next_customers = NULL;
        return;
    }
    g_next_customers = malloc(g_length_next_customers * sizeof(Customer));
    for (int i = 0; i < g_length_next_customers; ++i) {
        g_next_customers[i] = generate_customer(max_customer_time);
    }
}

int sum_customers_in_queue() {
    int res = 0;
    for (int i = 0; i < max_sum_cashiers; ++i) {
        res += g_cashiers[i].queue->length;
    }
    return res;
}

int sum_served_customers() {
    int res = 0;
    for (int i = 0; i < max_sum_cashiers; ++i) {
        res += g_cashiers[i].sum_served_customers;
    }
    return res;
}

int sum_working_cashier() {
    int res = 0;
    for (int i = 0; i < max_sum_cashiers; ++i) {
        res += g_cashiers[i].is_working;
    }
    return res;
}

bool is_overflow() {
    int sum = 0;
    int max_sum = max_queue_length * max_sum_cashiers;
    for (int i = 0; i < max_sum_cashiers; ++i) {
        sum += g_cashiers[i].queue->length;
    }
    return (sum == max_sum) ? true : false;
}
