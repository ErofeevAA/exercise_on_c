#include "film.h"


#define FILE_ALL_FILM_NAME "films.txt"

typedef struct NodeFilm {
    Film film;
    struct NodeFilm *prev;
    struct NodeFilm *next;
} NodeFilm;

typedef struct {
    NodeFilm *head;
    NodeFilm *tail;
} ListFilm;

void push_film(ListFilm *list, Film film, char *name);

void pop_film(ListFilm *list, Film film, char *name);

ListFilm load_films(char *name);

ListFilm load_film_with_check(ListFilm main, char *name);

void free_all_list_film(ListFilm *list);

bool is_exist_film(ListFilm list, Film film);