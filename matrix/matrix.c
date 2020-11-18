#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "matrix.h"


struct matrix create(int n, int m) {
    struct matrix res;
    res.n = n;
    res.m = m;
    srand(n * m);
    res.arr = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        res.arr[i] = (int *) malloc(m * sizeof(int));
        for (int j = 0; j < m; ++j) {
            res.arr[i][j] = rand() % 100;
        }
    }
    return res;
}

void refactor_main_diag(struct matrix *val, ...) {
    int length = (val->n < val->m) ? val->n : val->m;
    va_list factor;
    va_start(factor, val);
    for (int i = 0; i < length; ++i) {
        val->arr[i][i] = va_arg(factor, int);
    }
}

void print(struct matrix val) {
    for (int i = 0; i < val.n; ++i) {
        for (int j = 0; j < val.m; ++j) {
            printf("%d ", val.arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void delete(struct matrix *val) {
    for (int i = 0; i < val->n; ++i) {
        free((*val).arr[i]);
    }
    free((*val).arr);
}

struct matrix sum(struct matrix first, struct matrix second) {
    struct matrix res;
    if (first.n != second.n && first.m != second.m) {
        res.arr = NULL;
        return res;
    }
    res.n = first.n;
    res.m = first.m;
    res.arr = (int **) malloc(first.n * sizeof(int *));
    for (int i = 0; i < first.n; ++i) {
        res.arr[i] = (int *) malloc(first.m * sizeof(int));
        for (int j = 0; j < first.m; ++j) {
            res.arr[i][j] = first.arr[i][j] + second.arr[i][j];
        }
    }
    return res;
}

struct matrix multiplication(struct matrix first, struct matrix second) {
    struct matrix res;
    if (first.m != second.n) {
        res.arr = NULL;
        return res;
    }
    res.m = first.n;
    res.n = second.m;
    res.arr = (int **) malloc(res.n * sizeof(int *));
    for (int i = 0; i < res.n; ++i) {
        res.arr[i] = (int *) malloc(res.m * sizeof(int));
        for (int j = 0; j < res.m; ++j) {
            int sum = 0;
            for (int k = 0; k < first.m; ++k) {
                sum += first.arr[i][k] * second.arr[k][j];
            }
            res.arr[i][j] = sum;
        }
    }
    return res;
}

struct matrix transpose(struct matrix val) {
    struct matrix res;
    res.n = val.m;
    res.m = val.n;
    res.arr = (int **) malloc(res.n * sizeof(int *));
    for (int i = 0; i < res.n; ++i) {
        res.arr[i] = (int *) malloc(res.m * sizeof(int));
        for (int j = 0; j < res.m; ++j) {
            res.arr[i][j] = val.arr[j][i];
        }
    }
    return res;
}