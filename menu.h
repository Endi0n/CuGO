#ifndef CUGO_MENU_H
#define CUGO_MENU_H

#include <SDL2/SDL.h>
#include "color_scheme.h"

typedef unsigned int uint_t;

bool menu_visible();
bool menu_visible(bool);

color_scheme_t menu_color_scheme();
uint_t menu_board_size();
bool menu_sound();

bool menu_button_pressed(SDL_MouseButtonEvent, SDL_Rect);

void menu_loop(SDL_Event);

#endif // CUGO_MENU_H
