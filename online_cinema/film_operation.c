//#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "film_operation.h"


bool equals_film();
bool del_from_file(Film film, char  *name);

void push_film(ListFilm *list, Film film, char *name) {
    const int LENGTH = 100;
    FILE *file = fopen(name, "a");
    if (ftell(file) > 0) {
        fprintf(file, "\n");
    }
    fprintf(file, "%s\n", film.name);
    fprintf(file, "%d\n", film.year);
    fprintf(file, "%s\n", film.country);
    fprintf(file, "%s\n", film.genre);
    fprintf(file, "%.1f", film.rating);
    fclose(file);
    if (list->head == NULL) {
        list->head = malloc(sizeof(NodeFilm));
        list->head->film.name = malloc(sizeof(char) * LENGTH);
        memcpy(list->head->film.name, film.name, LENGTH);
        list->head->film.year = film.year;
        list->head->film.country = malloc(sizeof(char) * LENGTH);
        memcpy(list->head->film.country, film.country, LENGTH);
        list->head->film.genre = malloc(sizeof(char) * LENGTH);
        memcpy(list->head->film.genre, film.genre, LENGTH);
        list->head->film.rating = film.rating;
        list->tail = list->head;
        list->head->prev = NULL;
        list->head->next = NULL;
        return;  
    }
    NodeFilm *tmp = malloc(sizeof(NodeFilm));
    tmp->film.name = malloc(sizeof(char) * LENGTH);
    memcpy(tmp->film.name, film.name, LENGTH);
    tmp->film.year = film.year;
    tmp->film.country = malloc(sizeof(char) * LENGTH);
    memcpy(tmp->film.country, film.country, LENGTH);
    tmp->film.genre = malloc(sizeof(char) * LENGTH);
    memcpy(tmp->film.genre, film.genre, LENGTH);
    tmp->film.rating = film.rating;
    tmp->prev = list->tail;
    list->tail->next = tmp;
    tmp->next = NULL;
    list->tail = tmp;
}

void pop_film(ListFilm *list, Film film, char *name) {
    if (!del_from_file(film, name)) {
        return;
    }
    NodeFilm *tmp = list->head;
    NodeFilm *prev = NULL;
    while (tmp != NULL) {
        if (equals_film(tmp->film, film)) {
            if (list->head == list->tail) {
                free(list->head->film.name);
                free(list->head->film.country);
                free(list->head->film.genre);
                free(list->head);
                list->head = NULL;
                list->tail = NULL;
                break;
            }
            if (prev == NULL) {
                free(list->head->film.name);
                free(list->head->film.country);
                free(list->head->film.genre);
                prev = list->head->next;
                free(list->head);
                list->head = prev;
                list->head->prev = NULL;
                break;
            }
            if (tmp->next == NULL) {
                free(list->tail->film.name);
                free(list->tail->film.country);
                free(list->tail->film.genre);
                free(list->tail);
                prev->next = NULL;
                list->tail = prev;
                break;
            }
            tmp->next->prev = prev;
            prev->next = tmp->next;
            free(tmp->film.name);
            free(tmp->film.country);
            free(tmp->film.genre);
            free(tmp);
            break;
        }
        prev = tmp;   
        tmp = tmp->next;
    }
}

ListFilm load_films(char *name) {
    FILE *file = fopen(name, "r");
    ListFilm res;
    res.head = NULL;
    res.tail = NULL;
    if (file == NULL) {
        return res;
    }
    fseek(file, 0, SEEK_END);
    if (ftell(file) < 1) {
        fclose(file);
        return res;
    }
    fseek(file, 0, SEEK_SET);
    const int LENGTH_STR = 100;
    res.head = malloc(sizeof(NodeFilm));
    NodeFilm *cur = res.head;
    NodeFilm *prev = NULL;
    while (true) {
        char line[10];
        char *pointer;
        cur->film.name = malloc(LENGTH_STR * sizeof(char));
        fgets(cur->film.name, LENGTH_STR, file);
        pointer = strchr(cur->film.name, '\n');
        *pointer = '\0';
        fgets(line, LENGTH_STR, file);
        cur->film.year = (int) strtol(line, NULL, 10);
        cur->film.country = malloc(LENGTH_STR *sizeof(char));
        fgets(cur->film.country, LENGTH_STR, file);
        pointer = strchr(cur->film.country, '\n');
        *pointer = '\0';
        cur->film.genre = malloc(LENGTH_STR * sizeof(char));
        fgets(cur->film.genre, LENGTH_STR, file);
        pointer = strchr(cur->film.genre, '\n');
        *pointer = '\0';
        fgets(line, LENGTH_STR, file);
        cur->film.rating = (double) strtod(line, NULL);
        cur->prev = prev;
        if (feof(file)) {
            cur->next = NULL;
            res.tail = cur;
            break;
        }
        prev = cur;
        cur->next = malloc(sizeof(NodeFilm));
        cur = cur->next;
    }
    fclose(file);
    return res;
}

ListFilm load_film_with_check(ListFilm main, char *name) {
    ListFilm res = load_films(name);
    NodeFilm *tmp = res.head;
    while (tmp != NULL) {
        if (!is_exist_film(main, tmp->film)) {
            NodeFilm *del = tmp;
            tmp = tmp->next;
            pop_film(&res, del->film, name);
            continue;
        }
        tmp = tmp->next;
    }
    return res;
}

void free_all_list_film(ListFilm *list) {
    if (list->head == NULL) {
        return;
    }
    NodeFilm *prev = list->head;
    NodeFilm *tmp = list->head->next;
    while(tmp != NULL) {
        free(prev->film.name);
        free(prev->film.country);
        free(prev->film.genre);
        free(prev);
        prev = NULL;
        prev = tmp;
        tmp = tmp->next;
    }
    free(prev->film.name);
    free(prev->film.country);
    free(prev->film.genre);
    free(prev);
    list->head = NULL;
    list->tail = NULL;
}

bool is_exist_film(ListFilm list, Film film) {
    NodeFilm *tmp = list.head;
    while (tmp != NULL) {
        if (equals_film(tmp->film, film)) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

bool equals_film(Film first, Film second) {
    if (first.year == second.year) {
        if (strcmp(first.name, second.name) == 0) {
            return true;
        }
    }
    return false;
}

bool del_from_file(Film film, char *name) {
    FILE *file = fopen(name, "r");
    if (file == NULL) {
        return false;
    }
    fseek(file, 0, SEEK_END);
    if (ftell(file) < 1) {
        fclose(file);
        return false;
    }
    fseek(file, 0, SEEK_SET);
    const int length = 100;
    bool flag = false;
    fseek (file,0,SEEK_END);
    char *data = malloc(ftell (file) + 1);
    char *ptr = data;
    *ptr = 0;
    fseek (file,0,SEEK_SET);
    char *name_n  = malloc(strlen(film.name) + 2);
    sprintf(name_n, "%s\n", film.name);
    while (!feof(file)) {
        char line[length];
        fgets(line, length, file);
        if (strcmp(name_n, line) == 0) {
            fgets(line, length, file);
            fgets(line, length, file);
            fgets(line, length, file);
            fgets(line, length, file);
            flag = true;
            continue;
        }
        strcat(ptr, line);
        ptr += strlen(line);
        for (int i = 0; i < 4; ++i) {
            fgets(line, length, file);
            strcat(ptr, line);
            ptr += strlen(line);
        }
    }
    fclose(file);
    ptr--;
    *ptr = '\0';
    free(name_n);
    file = fopen(name, "w");
    fputs(data, file);
    fclose(file);
    free(data);
    return flag;
}