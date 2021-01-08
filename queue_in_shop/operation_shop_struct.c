#include <stdlib.h>
#include <time.h>
#include "operation_shop_struct.h"

void free_cashier_queue(Cashier *cashier);

Customer generate_customer(int max_time) {
    Customer result;
    result.name = (char) (rand() % 24 + 97);
    result.time = rand() % max_time + 1;
    return result;
}

void push_customer(Cashier *cashier, Customer customer) {
    if (cashier->queue->length == 0) {
        cashier->is_working = true;
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
    tmp = tmp->next;
    cashier->queue->tail = tmp;
    (cashier->queue->length)++;
}

void pop_customer(Cashier *cashier) {
    NodeCustomer *tmp = cashier->queue->head->next;
    free(cashier->queue->head);
    cashier->queue->head = tmp;
    (cashier->queue->length)--;
    (cashier->sum_served_customers)++;
    if (cashier->queue->head == NULL) {
        cashier->queue->tail = NULL;
        cashier->is_working = false;
    }
}

void free_all_cashiers_queue(Cashier **cashiers, int length) {
    Cashier *ptr = *cashiers;
    free_cashier_queue(ptr);
    for (int i = 1; i < length; ++i) {
        ptr++;
        free_cashier_queue(ptr);
    }
}

void free_cashier_queue(Cashier *cashier) {
    NodeCustomer *prev;
    NodeCustomer  *tmp = cashier->queue->head;
    while (tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
    free(cashier->queue);
    cashier->queue = NULL;
}
