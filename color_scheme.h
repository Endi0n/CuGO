#ifndef CUGO_COLOR_SCHEME_H
#define CUGO_COLOR_SCHEME_H

#include <SDL2/SDL.h>

struct color_scheme_t {
    SDL_Color board_cell_colors[2];
    SDL_Color player_piece_colors[2];
};

#endif // CUGO_COLOR_SCHEME_H
