#pragma once
#include <stdbool.h>

typedef struct {
    char name;
    int time;
} Customer;

typedef struct NodeCustomer {
    Customer visitor;
    struct NodeCustomer *next;
} NodeCustomer;

typedef struct {
    int length;
    NodeCustomer *head;
    NodeCustomer *tail;
} QueueCustomer;

typedef struct {
    bool is_working;
    int sum_all_customers;
    QueueCustomer *queue;
} Cashier;