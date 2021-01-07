#include <stdlib.h>
#include <time.h>
#include "operation_shop_struct.h"

Customer generate_customer(int max_time) {
    srand(time(0));
    Customer result;
    result.name = (char) (rand() % 24 + 97);
    result.time = rand() % max_time + 1;
    return result;
}

void push_customer(Cashier *cashier, Customer customer) {
    if (cashier->queue == NULL) {
        cashier->queue = malloc(sizeof(QueueCustomer));
        cashier->queue->length = 0;
    }
    if (cashier->queue->length == 0) {
        cashier->queue->head = malloc(sizeof(NodeCustomer));
        cashier->queue->length = 1;
        cashier->queue->head->customer = customer;
        cashier->queue->head->next = NULL;
        cashier->queue->tail = cashier->queue->head;
        return;
    }
    NodeCustomer *tmp = cashier->queue->tail;
    tmp->next = malloc(sizeof(NodeCustomer));
    tmp->next->customer = customer;
    tmp->next->next = NULL;
    cashier->queue->tail = tmp;
    (cashier->queue->length)++;
}

void pop_customer(Cashier *cashier) {
    NodeCustomer *tmp = cashier->queue->head->next;
    free(cashier->queue->head);
    cashier->queue->head = tmp;
    (cashier->queue->length)--;
}

void free_all_queue_customers(Cashier *cashier) {
    NodeCustomer *prev;
    NodeCustomer  *tmp = cashier->queue->head;
    while (tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
}
