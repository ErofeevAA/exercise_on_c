#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "user_operation.h"


char *generate_ip();
char *generate_device();
void count_middle(ListUser *list);
bool check(User object, User param);

User generate() {
    User res;
    srandom(time(NULL));
    res.IP = generate_ip();
    res.device = generate_device();
    res.age = (int) random() % 99 + 1;
    res.depth = (int) random() % 10 + 1;
    return res;
}

void push(ListUser *list, User user) {
    NodeUser *tmp = list->head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = malloc(sizeof(NodeUser));
    tmp->next->next = NULL;
    tmp->next->user = user;
    list->sum_user++;
    count_middle(list);
}

void pop(ListUser *list, User user) {
    NodeUser *tmp = list->head;
    NodeUser *prev;
    while (tmp != NULL) {
        if (check(tmp->user, user)) {
            (list->sum_user)--;
            if (tmp == list->head) {
                tmp = tmp->next;
                free(list->head);
                list->head = tmp;
                continue;
            }
            prev->next = tmp->next;
            free(tmp);
            tmp = NULL; //??? вроде всё работает и так, но clion ругается без этого
            //говорит, что могу ссылаться к освобождённой памяти
            //хотя, возможно, это баг редактора
            tmp = prev->next;
            continue;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    count_middle(list);
}

ListUser copy(ListUser list) {
    ListUser res;
    res.sum_user = list.sum_user;
    res.mid_age = list.mid_age;
    res.mid_depth = list.mid_depth;
    res.head = malloc(sizeof(NodeUser));
    NodeUser *tmp_in = list.head;
    NodeUser *tmp_out = res.head;
    while (tmp_in->next != NULL) {
        tmp_out->user = tmp_in->user;
        tmp_out->next = malloc(sizeof(NodeUser));
        tmp_out = tmp_out->next;
        tmp_in = tmp_in->next;
    }
    tmp_out->user = tmp_in->user;
    tmp_out->next = NULL;
    return res;
}

ListUser filter(ListUser list, User user) {
    ListUser res;
    res.sum_user = 0;
    res.head = NULL;
    NodeUser *tmp_in = list.head;
    NodeUser *tmp_out = malloc(sizeof(NodeUser));
    while (tmp_in != NULL) {
        if (check(tmp_in->user, user)) {
            (res.sum_user)++;
            if (res.head == NULL) {
                tmp_out->user = tmp_in->user;
                res.head = tmp_out;
                tmp_out->next = malloc(sizeof(NodeUser));
                tmp_out = tmp_out->next;
                tmp_in = tmp_in->next;
                continue;
            }
            if (tmp_in->next == NULL) {
                tmp_out->user = tmp_in->user;
                tmp_out->next = NULL;
                break;
            }
            tmp_out->user = tmp_in->user;
            tmp_out->next = malloc(sizeof(NodeUser));
            tmp_out = tmp_out->next;
        }
        tmp_in = tmp_in->next;
    }
    count_middle(&res);
    return res;
}

void print_list(ListUser list) {
    printf("Вывод списка\n");
    NodeUser *tmp = list.head;
    int count  = 0;
    while (tmp != NULL) {
        printf("Пользатель номер %d:\n", ++count);
        printf("IP: %s\n", tmp->user.IP);
        printf("Device: %s\n", tmp->user.device);
        printf("Age: %d\n", tmp->user.age);
        printf("Depth: %d\n", tmp->user.depth);
        tmp = tmp->next;
    }
    printf("\n");
}

ListUser read_file(const char *name) {
    const int MAX_LENGTH_LINE = 17;
    FILE *file = fopen(name, "r");
    ListUser res;
    res.head = malloc(sizeof(NodeUser));
    res.sum_user = 0;
    int sum_depth = 0;
    int sum_age = 0;
    NodeUser *tmp = res.head;
    res.sum_user = 0;
    while (1) {
        (res.sum_user)++;
        tmp->user.IP = malloc(MAX_LENGTH_LINE * sizeof(char));
        fgets(tmp->user.IP, MAX_LENGTH_LINE, file);
        char *p = strchr(tmp->user.IP, '\n');
        *p = '\0';
        tmp->user.device = malloc(MAX_LENGTH_LINE * sizeof(char));
        fgets(tmp->user.device, MAX_LENGTH_LINE, file);
        p = strchr(tmp->user.device, '\n');
        *p = '\0';

        char *line = malloc(4 * sizeof(char ));
        fgets(line, 4, file);
        tmp->user.age = (int) strtol(line, NULL, 10);
        sum_age += tmp->user.age;
        fgets(line, 4, file);
        tmp->user.depth = (int) strtol(line, NULL, 10);
        sum_depth += tmp->user.depth;
        free(line);

        if (feof(file)) {
            tmp->next = NULL;
            break;
        }
        tmp->next = malloc(sizeof(NodeUser));
        tmp = tmp->next;
    }
    res.mid_depth = (double) sum_depth / res.sum_user;
    res.mid_age = (double) sum_age / res.sum_user;
    fclose(file);
    return res;
}

void print_state(ListUser list) {
    printf("Вывод статы списка\n");
    printf("Всего пользователей: %d\n", list.sum_user);
    printf("Средний возраст: %.2f\n", list.mid_age);
    printf("Средняя глубина посещения: %.2f\n", list.mid_depth);
}

char *generate_ip() {
    char *res = malloc(16 * sizeof(char));
    srandom(time(NULL));
    int cur_byte = (int) random() % 256;
    sprintf(res, "%d", cur_byte);
    for (int i = 1; i < 4; ++i) {
        cur_byte = (int) random() % 256;
        sprintf(res, "%s.%d", res, cur_byte);
    }
    sprintf(res, "%s%c", res, '\0');
    return res;
}

char *generate_device() {
    srandom(time(NULL));
    switch ((int) random() % 5) {
        case 0:
            return "Windows\0";
        case 1:
            return "MacOS\0";
        case 2:
            return "Linux\0";
        case 3:
            return "iOS\0";
        default:
            return "Android\0";
    }
}

void count_middle(ListUser *list) {
    int sum_age = 0;
    int sum_deep = 0;
    NodeUser *tmp = list->head;
    while (tmp != NULL) {
        sum_age += tmp->user.age;
        sum_deep += tmp->user.depth;
        tmp = tmp->next;
    }
    list->mid_age = (double) sum_age / list->sum_user;
    list->mid_depth = (double) sum_deep / list->sum_user;
}

bool check(User object, User param) {
    if (param.IP != NULL && strcmp(object.IP, param.IP) == 0) {
        return true;
    }
    if (param.device != NULL && strcmp(object.device, param.device) == 0) {
        return true;
    }
    if (object.age == param.age) {
        return true;
    }
    if (object.depth == param.depth) {
        return true;
    }
    return false;
}