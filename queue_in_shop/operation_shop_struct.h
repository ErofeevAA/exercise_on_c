#include "shop_struct.h"

Customer generate_customer(int max_time);

void push_customer(Cashier *cashier, Customer customer);

void pop_customer(Cashier *cashier);

void free_all_queue_customers(Cashier *cashier);