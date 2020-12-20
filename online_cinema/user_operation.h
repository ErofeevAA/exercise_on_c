#include <stdbool.h>
#include "user.h"

#define MAX_LENGTH_F_USER_STR 21

typedef struct NodeUser {
    User user;
    struct NodeUser* next;
} NodeUser;

typedef struct {
    NodeUser *head;
} ListUser;

void push_user(ListUser *list, User user);

ListUser load_users();

void free_all_list_user(ListUser *list);

User search_user(ListUser list, char *login, char *password);

bool is_exist_user(ListUser list, char *login);

char *get_file_favour_name(char *login);