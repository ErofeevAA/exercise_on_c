struct matrix {
    int n, m;
    int **arr;
};

struct matrix create(int, int);

void refactor_main_diag(struct matrix *val, ...);

void print(struct matrix);

void delete(struct matrix *);

struct matrix sum(struct matrix, struct matrix);

struct matrix multiplication(struct matrix, struct matrix);

struct matrix transpose(struct matrix);