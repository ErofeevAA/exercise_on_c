typedef struct User {
    char *IP;
    char *device;
    int age;
    int depth;
} User;

typedef struct NodeUser {
    User user;
    struct NodeUser *next;
} NodeUser;

typedef struct ListUser {
    NodeUser *head;
    int sum_user;
    double mid_age;
    double mid_depth;
} ListUser;