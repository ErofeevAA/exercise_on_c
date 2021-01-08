#include "shop_struct.h"

void init_coords(int max_queue_length);

void draw_head();

void draw_all_cashiers(int max_items, int sum_cashiers, Cashier *cashiers);

void draw_time(int time);

void draw_next_customers(int length, Customer *customers);

void draw_sum_customers(int sum);

void draw_sum_working_cashiers(int sum, int max);

void draw_sum_served_customers(int sum);

void draw_max_queue_cashier_length(int max);