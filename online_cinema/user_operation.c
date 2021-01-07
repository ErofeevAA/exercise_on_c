#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user_operation.h"


#define FILE_NAME "user.txt"
#define FILE_FAVOUR "favour_film_"
#define FILE_EXT ".txt"

void write_user_to_file(User user);

void push_user(ListUser *list, User user) {
    if (list->head == NULL) {
        list->head = malloc(sizeof(NodeUser));
        list->head->user = user;
        list->head->next = NULL;
        write_user_to_file(user);
        return;
    }
    NodeUser *tmp = list->head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = malloc(sizeof(NodeUser));
    tmp->next->user = user;
    tmp->next->next = NULL;
    write_user_to_file(user);
}

ListUser load_users() {
    const int CARD_LENGTH = 18;
    ListUser result;
    result.head = NULL;
    FILE *file;
    file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return result;
    }
    result.head = malloc(sizeof(NodeUser));
    NodeUser *tmp = result.head;
    while (true) {
        char *p;
        tmp->user.login = malloc(1 + MAX_LENGTH_F_USER_STR * sizeof(char));
        fgets(tmp->user.login, MAX_LENGTH_F_USER_STR + 1, file);
        p = strchr(tmp->user.login, '\n');
        *p = '\0';
        tmp->user.password = malloc(1 + MAX_LENGTH_F_USER_STR * sizeof(char));
        fgets(tmp->user.password, MAX_LENGTH_F_USER_STR + 1, file);
        p = strchr(tmp->user.password, '\n');
        *p = '\0';
        char *line = malloc(CARD_LENGTH * sizeof(char));
        fgets(line, CARD_LENGTH, file);
        tmp->user.is_admin = strtol(line, NULL, 10);
        fgets(line, CARD_LENGTH, file);
        tmp->user.card = strtol(line, NULL, 10);
        if (feof(file)) {
            tmp->next = NULL;
            break;
        }
        tmp->next = malloc(sizeof(NodeUser));
        tmp = tmp->next;
    }
    fclose(file);
    return result;
}

void free_all_list_user(ListUser *list) {
    if (list->head == NULL) {
        return;
    }
    NodeUser *prev;
    NodeUser *tmp = list->head;
    while (tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
        free(prev->user.login);
        free(prev->user.password);
        free(prev);
    }
}

User search_user(ListUser list, char *login, char *password) {
    User result;
    result.login = NULL;
    result.password = NULL;
    NodeUser *tmp = list.head;
    while (tmp != NULL) {
        if (strcmp(login, tmp->user.login) == 0) {
            if (strcmp(password, tmp->user.password) == 0) {
                result.login = login;
                result.password = password;
                result.card = tmp->user.card;
                result.is_admin = tmp->user.is_admin;
                break;
            }
        }
        tmp = tmp->next;
    }
    return result;
}

bool is_exist_user(ListUser list, char *login) {
    NodeUser *tmp = list.head;
    while (tmp != NULL) {
        if (strcmp(login, tmp->user.login) == 0) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

char *get_file_favour_name(char *login) {
    char *res = malloc(sizeof(char) * 50);
    snprintf(res, 50, "%s%s%s", FILE_FAVOUR, login, FILE_EXT);
    return res;
}

void write_user_to_file(User user) {
    FILE *file;
    file = fopen(FILE_NAME, "a");
    fprintf(file, "\n%s", user.login);
    fprintf(file, "\n%s", user.password);
    fprintf(file, "\n%d", user.is_admin);
    fprintf(file, "\n%lld", user.card);
    fclose(file);
}
