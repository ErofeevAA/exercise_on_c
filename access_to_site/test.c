#include <stdio.h>
#include "test.h"
#include "user_operation.h"

const char *NAME = "/home/artyom/CLionProjects/access_to_site/input.txt";


void test_generate() {
    printf("Test generate()\n");
    User user = generate();
    printf("%s\n", user.IP);
    printf("%s\n", user.device);
    printf("%d\n", user.age);
    printf("%d\n", user.depth);
    printf("\n\n");
}

void test_push() {
    printf("Test push()\n");
    ListUser list = read_file(NAME);
    push(&list, generate());
    print_state(list);
    print_list(list);
    printf("\n");
}

void test_pop() {
    printf("Test pop()\n");
    ListUser list = read_file(NAME);
    User user;
    user.device = "Windows\0";
    user.IP = NULL;
    user.depth = 0;
    user.age = 0;
    pop(&list, user);
    print_state(list);
    print_list(list);
    printf("\n");
}

void test_copy() {
    printf("test copy()\n");
    ListUser list1 = read_file(NAME);
    ListUser list2 = copy(list1);
    print_state(list2);
    print_list(list2);
    printf("\n");
}

void test_filter() {
    printf("test filter()\n");
    ListUser list1 = read_file(NAME);
    User user;
    user.IP = NULL;
    user.device = "Android\0";
    user.age = 0;
    user.depth = 1;
    ListUser list2 = filter(list1, user);
    print_state(list2);
    print_list(list2);
    printf("\n");
}

void test_read_file() {
    printf("test read_file()\n");
    ListUser list = read_file(NAME);
    print_state(list);
    print_list(list);
    printf("\n");
}