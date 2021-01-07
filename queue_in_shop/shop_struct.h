#pragma once
#include <stdbool.h>

typedef struct {
    char name;
    int time;
} Customer;

typedef struct NodeCustomer {
    Customer customer;
    struct NodeCustomer *next;
} NodeCustomer;

typedef struct {
    int length;
    NodeCustomer *head;
    NodeCustomer *tail;
} QueueCustomer;

typedef struct {
    bool is_working;
    int sum_served_customers;
    QueueCustomer *queue;
} Cashier;