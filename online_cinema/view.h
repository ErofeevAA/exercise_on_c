#include "film.h"


#define NOT_INPUT_STATE 200
#define INPUT_STATE 201
#define INVALID_INPUT_STATE 202
#define EXIST_INPUT_STATE 203
#define COMPLETE_STATE 204
#define ADMIN_STATE 205
#define USER_STATE 206

#define SIGN_IN_WINDOW 300
#define SIGN_UP_WINDOW 301
#define CATALOG_WINDOW 302
#define FAVOUR_CATALOG_WINDOW 303
#define ADD_FILM_WINDOW 304

void draw_hint(int state);

void draw_head(int window);
void draw_input_login();
void draw_input_password();
void draw_input_card();

void draw_input_name();
void draw_input_year();
void draw_input_country();
void draw_input_genre();
void draw_input_rating();

void draw_film_cards(char left_sym, Film film, char right_sym);
void draw_more_info(Film film);
void clear_more_info();