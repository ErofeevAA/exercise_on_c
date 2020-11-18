#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "matrix.h"

struct matrix get_matrix() {
    struct matrix res;
    res.m = 3;
    res.n = 3;
    res.arr = (int **) malloc(res.n * sizeof(int *));
    for (int i = 0; i < res.n; ++i) {
        res.arr[i] = (int *) malloc(res.m * sizeof(int));
        for (int j = 0; j < res.m; ++j) {
            res.arr[i][j] = j;
        }
    }
    return res;
}

void test_create() {
    printf("Testing function create\n");
    struct matrix v = create(10, 10);
    print(v);
    delete(&v);
    printf("Successful\n\n\n\n");
}

void test_refactor() {
    printf("Test function refactor\n");
    struct matrix v = create(3, 2);
    printf("Initial matrix\n");
    print(v);
    printf("Refactor matrix (param 1, 2)\n");
    refactor_main_diag(&v, 1, 2);
    print(v);
    delete(&v);
    v = create(2, 3);
    printf("Initial matrix\n");
    print(v);
    printf("Refactor matrix (param 1, 2)\n");
    refactor_main_diag(&v, 1, 2);
    print(v);
    delete(&v);
    printf("Successful\n\n\n\n");
}

void test_sum() {
    printf("Test function sum\n");
    struct matrix f, s, r;
    f = get_matrix();
    s = get_matrix();
    printf("Initial matrices\n");
    print(f);
    print(s);
    printf("Expect\n");
    printf("0 2 4\n0 2 4\n0 2 4\n\n");
    printf("Result\n");
    r = sum(f, s);
    print(r);
    delete(&f);
    delete(&s);
    delete(&r);
    printf("Successful\n\n\n\n");
}

void test_multiplication() {
    printf("Test function multiplication\n");
    struct matrix f = get_matrix();
    struct matrix s = get_matrix();
    printf("Initial matrices\n");
    print(f);
    print(s);
    printf("Expect\n");
    printf("0 3 6\n0 3 6\n0 3 6\n\n");
    printf("Result\n");
    struct matrix r = multiplication(f, s);
    print(r);
    delete(&f);
    delete(&s);
    delete(&r);
    printf("Successful\n\n\n\n");
}

void test_transpose() {
    printf("Test function transpose\n");
    struct matrix v = get_matrix();
    printf("Initial matrices\n");
    print(v);
    printf("Expect\n");
    printf("0 0 0\n1 1 1\n2 2 2\n\n");
    struct matrix r = transpose(v);
    printf("Result\n");
    print(r);
    delete(&v);
    delete(&r);
    printf("Successful\n\n\n\n");
}