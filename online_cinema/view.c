#include <ncurses.h>
#include "view.h"


static int g_Window = SIGN_IN_WINDOW;

void get_hint_sign_in(int state, char **first, char **second);
void get_hint_sign_up(int state, char **first, char **second);
void get_hint_catalog(int state, char **first, char **second, char **third);
void get_hint_favour_catalog(char **first, char **second, char **third);
void get_hint_add_film(int state, char **first, char **second);

void get_coord_hint(int *row, int *column);

void win_draw_sym_in_center(WINDOW **win, char sym);

void draw_head(int window) {
    clear();
    g_Window = window;
    const int ROW = 1;
    const int COLUMN = 10;
    attron(A_BOLD);
    switch (window) {
        case SIGN_IN_WINDOW:
            mvprintw(ROW, COLUMN, "Sign in");
            break;
        case SIGN_UP_WINDOW:
            mvprintw(ROW, COLUMN, "Sign up");
            break;
        case CATALOG_WINDOW:
            mvprintw(ROW, COLUMN, "Catalog");
            break;
        case FAVOUR_CATALOG_WINDOW:
            mvprintw(ROW, COLUMN, "Favourites Catalog");
            break;
        case ADD_FILM_WINDOW:
            mvprintw(ROW, COLUMN, "Add Film");
            break;
        default:
            mvprintw(ROW, COLUMN, "Error");
            break;
    }
    attroff(A_BOLD);
    refresh();
}

void draw_hint(int state) {
    char *first_hint;
    char *second_hint;
    char *third_hint = NULL;
    int row;
    int column;
    switch (g_Window) {
        case SIGN_IN_WINDOW:
            get_hint_sign_in(state, &first_hint, &second_hint);
            break;
        case SIGN_UP_WINDOW:
            get_hint_sign_up(state, &first_hint, &second_hint);
            break;
        case CATALOG_WINDOW:
            get_hint_catalog(state, &first_hint, &second_hint, &third_hint);
            break;
        case FAVOUR_CATALOG_WINDOW:
            get_hint_favour_catalog(&first_hint, &second_hint, &third_hint);
            break;
        case ADD_FILM_WINDOW:
            get_hint_add_film(state, &first_hint, &second_hint);
            break;
        default:
            printw("Error");
            return;
    }
    attron(A_ITALIC);
    get_coord_hint(&row, &column);
    move(row, column);
    clrtoeol();
    printw(first_hint);
    move(row + 1, column);
    clrtoeol();
    printw(second_hint);
    if (third_hint != NULL) {
        move(row + 2, column);
        printw(third_hint);
    }
    attroff(A_ITALIC);
    refresh();
}

void draw_input_login() {
    clrtoeol();
    printw("Login: ");
    echo();
    refresh();
}

void draw_input_password() {
    printw("Password: ");
    noecho();
    refresh();
}

void draw_input_card() {
    clrtoeol();
    printw("Card: ");
    echo();
    refresh();
}

void draw_input_name() {
    clrtoeol();
    printw("Name: ");
    echo();
    refresh();
}

void draw_input_year() {
    clrtoeol();
    printw("Year: ");
    refresh();
}

void draw_input_country() {
    clrtoeol();
    printw("Country: ");
    refresh();
}

void draw_input_genre() {
    clrtoeol();
    printw("Genre: ");
    refresh();
}

void draw_input_rating() {
    clrtoeol();
    printw("Rating: ");
    refresh();
}

void draw_film_cards(char left_sym, Film film, char right_sym) {
    const int start_y = 5;
    const int height = 13;
    const int width = 20;
    const int left_x = 1;
    const int center_x = left_x + width;
    const int right_x = center_x + 2 + width;
    
    WINDOW *win_l  = newwin(height, width, start_y, left_x);
    WINDOW *win_c = newwin(height + 1, width + 2, start_y, center_x);
    WINDOW *win_r = newwin(height, width, start_y, right_x);
    box(win_l, 0, 0);
    box(win_c, 0, 0);
    box(win_r, 0, 0);

    win_draw_sym_in_center(&win_l, left_sym);
    win_draw_sym_in_center(&win_r, right_sym);

    WINDOW *win_c_text = newwin(win_c->_maxy - 1, win_c->_maxx - 1, start_y + 1, center_x + 1);
    wmove(win_c_text, win_c_text->_maxy / 2, 0);
    wprintw(win_c_text, "%s", film.name);
    
    refresh();
    wrefresh(win_l);
    wrefresh(win_c);
    wrefresh(win_c_text);
    wrefresh(win_r);
    delwin(win_l);
    delwin(win_c);
    delwin(win_c_text);
    delwin(win_r);
}

void draw_more_info(Film film) {
    const int start_y = 19;
    const int start_x = 1;
    const int height = 6;
    const int width = 40;
    int str_out_x = 1;
    WINDOW *win = newwin(height, width, start_y, start_x);
    box(win, 0, 0);
    wmove(win, str_out_x++, 1);
    wprintw(win, "Country: %s", film.country);
    wmove(win, str_out_x++, 1);
    wprintw(win, "Genre: %s", film.genre);
    wmove(win, str_out_x++, 1);
    wprintw(win, "Year: %d", film.year);
    wmove(win, str_out_x, 1);
    wprintw(win, "Rating: %.1f", film.rating);
    refresh();
    wrefresh(win);
    delwin(win);
}

void clear_more_info() {
    int i;
    for (i = 19; i < 19 + 6; ++i) {
        move(i, 0);
        clrtoeol();
    }
}

void get_hint_sign_in(int state, char **first, char **second) {
    switch (state) {
        case NOT_INPUT_STATE:
            *first = "Press ctrl R to sign up";
            *second = "Press any key to sign in (beside ESC)";
            break;
        case INPUT_STATE:
            *first = "Input login and password";
            *second = "\n";
            break;
        case COMPLETE_STATE:
            *first = "Registration complete. Press any key";
            *second = "to input (beside ESQ)";
            break;
        case INVALID_INPUT_STATE:
            *first = "Invalid input. Press ctrl R to sign up";
            *second = "Press any key to input (beside ESC)";
            break;
        default:
            printw("Error");
            break;
    }
}

void get_hint_sign_up(int state, char **first, char **second) {
    switch (state) {
        case NOT_INPUT_STATE:
            *first = "Press ctrl L to sign in";
            *second = "Press any key to start sign up (beside ESC)";
            break;
        case INPUT_STATE:
            *first = "Input login, password, card. Length of ";
            *second = "login 3-20, card\'s number 16, password :6-20\n";
            break;
        case INVALID_INPUT_STATE:
            *first = "Invalid input. Press ctrl L to sign in";
            *second = "Press any key to input (beside ESC)";
            break;
        case EXIST_INPUT_STATE:
            *first = "User already exist. Press ctrl L to sign in";
            *second = "Press any key to input (beside ESC)";
            break;
        default:
            printw("Error");
            break;
    }
}

void get_hint_catalog(int state, char **first, char **second, char **third) {
    *first = "Press arrow left/right to move. Press enter to get more about film";
        if (state == ADMIN_STATE) {
            *second = "Press DEL to remove. Press CTRL F to go to your catalog";
            *third = "Press + to add new film to all catalog and * to add to favourite";
            return;
        }
        *second = "Press CTRL F to go to your catalog. Press * to add film to favourite";
        *third = "Press ENTER to see more information about film";
}

void get_hint_favour_catalog(char **first, char **second, char **third) {
    *first = "Press arrow left/right to move. Press enter to get more about film";
    *second = "Press DEL to remove film. Press CTRL B to go to all catalog";
    *third = "Press ENTER to see more information about film";
}

void get_hint_add_film(int state, char **first, char **second) {
    if (state == NOT_INPUT_STATE) {
        *first = "Press any key to start input";
        *second = "Press CTRL B to go back";
        return;
    }
    if (state == INPUT_STATE) {
        *first = "Input data";
        *second = "\n";
        return;
    }
    if (state == COMPLETE_STATE) {
        *first = "Press any key to start input";
        *second = "Press CTRL B to go back";
        return;
    }
    *first = "Error";
    *second = "\n";
}

void get_coord_hint(int *row, int *column) {
    *row = 2;
    *column = 0;
}

void win_draw_sym_in_center(WINDOW **win, char sym) {
    wmove(*win, (**win)._maxy / 2, (**win)._maxx / 2);
    attron(A_BOLD);
    wprintw(*win, "%c", sym);
    attroff(A_BOLD);
}