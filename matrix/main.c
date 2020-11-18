#include <stdio.h>

#include "matrix.h"
#include "test.h"


int main() {
    test_create();
    test_refactor();
    test_sum();
    test_multiplication();
    test_transpose();
    printf("Hello, World!\n");
    return 0;
}