#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "view.h"
#include "user_operation.h"
#include "film_operation.h"
#include "cinema.h"

void controller();

void sign_in(void (**model)(), int key);
void sign_up(void (**model)(), int key);
void catalog(void (**model)(), int key);
void favour_catalog(void (**model)(), int key);
void add_film_to_catalog(void (**model)(), int key);

bool is_correct_in(char *login, char *password, long long card);

void cards_catalog_action(int key);

static ListUser g_list_user;
static User g_user;
static ListFilm g_list_film;
static ListFilm g_list_fav_film;
static NodeFilm *g_node_film;

void init_cinema() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    g_list_user = load_users();
    g_list_film.head = NULL;
    g_list_film.tail = NULL;
    g_list_fav_film.head = NULL;
    g_list_fav_film.tail = NULL;
    controller();
}

void destroy_cinema() {
    free_all_list_user(&g_list_user);
    free_all_list_film(&g_list_film);
    free_all_list_film(&g_list_fav_film);
    if (g_user.login != NULL) {
        free(g_user.login);
    }
    endwin();
}

void controller() {
    const int KEY_ESC = 27;
    void (*cur_model)() = sign_in;
    draw_head(SIGN_IN_WINDOW);
    draw_hint(NOT_INPUT_STATE);
    int key = getch();
    while (key != KEY_ESC) {
        //mvprintw(30, 0, "%d", key);
        cur_model(&cur_model, key);
        key = getch();
    }
}

void sign_in(void (**model)(), int key) {
    const int CTRL_R = 18;
    if (key == CTRL_R) {
        *model = sign_up;
        draw_head(SIGN_UP_WINDOW);
        draw_hint(NOT_INPUT_STATE);
        return;
    }
    char *login = malloc(MAX_LENGTH_F_USER_STR * sizeof(char));
    char password[MAX_LENGTH_F_USER_STR];
    draw_hint(INPUT_STATE);
    draw_input_login();
    scanw("%s", login);
    draw_input_password();
    getstr(password);
    g_user = search_user(g_list_user, login, password);
    if (g_user.login != NULL) {
        *model = catalog;
        clear();
        g_list_film = load_films(FILE_ALL_FILM_NAME);
        g_node_film = g_list_film.head;
        char *filename = get_file_favour_name(g_user.login);
        g_list_fav_film = load_films(filename);
        free(filename);
        draw_head(CATALOG_WINDOW);
        char r = (g_node_film->next == NULL) ?  'x' : '>';
        draw_film_cards('x', g_node_film->film, r);
        if (g_user.is_admin) {
            draw_hint(ADMIN_STATE);
            return;
        }
        draw_hint(USER_STATE);
        return;
    }
    free(login);
    draw_hint(INVALID_INPUT_STATE);
}

void sign_up(void (**model)(), int key) {
    const int CTRL_L = 12;
    if (key == CTRL_L) {
        *model = sign_in;
        draw_head(SIGN_IN_WINDOW);
        draw_hint(NOT_INPUT_STATE);
        return;
    }
    draw_hint(INPUT_STATE);
    char login[MAX_LENGTH_F_USER_STR];
    long long card;
    char password[MAX_LENGTH_F_USER_STR];
    draw_input_login();
    scanw("%s", login);
    draw_input_card();
    scanw("%lld", &card);
    draw_input_password();
    getstr(password);
    if (!is_correct_in(login, password, card)) {
        draw_hint(INVALID_INPUT_STATE);
        return;
    }
    if (!is_exist_user(g_list_user, login)) {
        User u;
        u.login = login;
        u.password = password;
        u.card = card;
        u.is_admin = false;
        push_user(&g_list_user, u);
        draw_head(SIGN_IN_WINDOW);
        draw_hint(COMPLETE_STATE);
        *model = sign_in;
        return;
    }
    draw_hint(EXIST_INPUT_STATE);
}

void catalog(void (**model)(), int key) {
    const int CTRL_F = 6;
    const int KEY_PLUS = 43;
    const int KEY_ASTERISK = 42;
    const int KEY_DEL = 330;
    if (key == CTRL_F) {
        free_all_list_film(&g_list_fav_film);
        char *filename = get_file_favour_name(g_user.login);
        g_list_fav_film = load_film_with_check(g_list_film, filename);
        free(filename);
        g_node_film = g_list_fav_film.head;
        *model = favour_catalog;
        clear();
        draw_head(FAVOUR_CATALOG_WINDOW);
        draw_hint(USER_STATE);
        cards_catalog_action(KEY_LEFT);
        return;
    }
    if (key == KEY_ASTERISK) {
        if (!is_exist_film(g_list_fav_film, g_node_film->film)) {
            char *name = get_file_favour_name(g_user.login);
            push_film(&g_list_fav_film, g_node_film->film, name);
            free(name);
        }
        return;
    }
    if (key == KEY_PLUS && g_user.is_admin) {
        *model = add_film_to_catalog;
        clear();
        draw_head(ADD_FILM_WINDOW);
        draw_hint(NOT_INPUT_STATE);
        return;
    }
    if (key == KEY_DEL && g_user.is_admin && g_node_film != NULL) {
        Film f = g_node_film->film;
        g_node_film = g_node_film->prev;
        pop_film(&g_list_film, f, FILE_ALL_FILM_NAME);
        if (g_node_film == NULL) {
            g_node_film = g_list_film.head;
        }
    }
    cards_catalog_action(key);
}

void favour_catalog(void (**model)(), int key) {
    const int CTRL_B = 2;
    const int KEY_DEL = 330;
    if (key == CTRL_B) {
        g_node_film = g_list_film.head;
        *model = catalog;
        clear();
        draw_head(CATALOG_WINDOW);
        cards_catalog_action(KEY_LEFT);
        if (g_user.is_admin) {
            draw_hint(ADMIN_STATE);
            return;
        }
        draw_hint(USER_STATE);
        return;
    }
    if (key == KEY_DEL && g_node_film != NULL) {
        Film f = g_node_film->film;
        g_node_film = g_node_film->prev;
        char *name = get_file_favour_name(g_user.login);
        pop_film(&g_list_fav_film, f, name);
        free(name);
        if (g_node_film == NULL) {
            g_node_film = g_list_film.head;
        }
    }
    cards_catalog_action(key);
}

void add_film_to_catalog(void (**model)(), int key) {
    const int CTRL_B = 2;
    const int LENGTH = 100;
    if (key == CTRL_B) {
        g_node_film = g_list_film.head;
        *model = catalog;
        clear();
        draw_head(CATALOG_WINDOW);
        draw_hint(ADMIN_STATE);
        cards_catalog_action(KEY_LEFT);
        return;
    }
    Film film;
    film.name = malloc(sizeof(char) * LENGTH);
    film.country = malloc(sizeof(char) * LENGTH);
    film.genre = malloc(sizeof(char) * LENGTH);
    draw_hint(INPUT_STATE);
    draw_input_name();
    getstr(film.name);
    draw_input_year();
    scanw("%d", &(film.year));
    draw_input_country();
    getstr(film.country);
    draw_input_genre();
    getstr(film.genre);
    draw_input_rating();
    char *r = malloc(sizeof(char) * 10);
    getstr(r);
    film.rating = strtod(r, NULL);
    free(r);
    push_film(&g_list_film, film, FILE_ALL_FILM_NAME);
    draw_hint(COMPLETE_STATE);
    noecho();
}

bool is_correct_in(char *login, char *password, long long int card) {
    if (strlen(login) < 3 || strlen(login) > 20) {
        return false;
    }
    if (strlen(password) < 6 || strlen(password) > 20) {
        return false;
    }
    char c[17];
    sprintf(c, "%lld", card);
    if (strlen(c) != 16) {
        false;
    }
    return true;
}

void cards_catalog_action(int key) {
    clear_more_info();
    if (g_node_film == NULL) {
        Film f;
        f.name = "Empty";
        draw_film_cards('x', f, 'x');
        return;
    }
    if (key == '\n') {
        draw_more_info(g_node_film->film);
    }
    if (key == KEY_LEFT) {
        if (g_node_film->prev != NULL) {
            g_node_film = g_node_film->prev;
        }
    }
    if (key == KEY_RIGHT) {
        if (g_node_film->next != NULL) {
            g_node_film = g_node_film->next;
        }
    }
    char l = (g_node_film->prev == NULL) ?  'x' : '<';
    char r = (g_node_film->next == NULL) ?  'x' : '>';
    draw_film_cards(l, g_node_film->film, r);
}