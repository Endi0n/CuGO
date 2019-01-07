#ifndef CUGO_MENU_H
#define CUGO_MENU_H

#include <SDL2/SDL.h>
#include "color_scheme.h"

typedef unsigned int uint;

void menu_show();
void menu_hide();
bool menu_visible();

uint menu_board_size();
color_scheme_t menu_color_scheme();

void menu_loop(SDL_Event);

#endif // CUGO_MENU_H
