#include "user_list.h"

User generate();

void push(ListUser *list, User user);

void pop(ListUser *list, User user);

ListUser copy(ListUser list);

ListUser filter(ListUser list, User user);

void print_list(ListUser list);

ListUser read_file(const char *name);

void print_state(ListUser list);