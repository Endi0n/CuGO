#ifndef CUGO_COLOR_SCHEME_H
#define CUGO_COLOR_SCHEME_H

#include <SDL2/SDL.h>

struct color_scheme_t {
    SDL_Color background;
    SDL_Color buttons_background;
    SDL_Color board_cell_colors[2];
    SDL_Color player_piece_colors[2];
};

const color_scheme_t color_schemes[] = {

    color_scheme_t {
        .background = {204, 223, 255},
        .buttons_background = {137, 164, 255},
        .board_cell_colors = {{100, 100, 100}, {200, 200, 200}},
        .player_piece_colors = {{0, 0, 0}, {255, 255, 255}}
    },


    color_scheme_t {
        .background = {204, 223, 255},
        .buttons_background = {137, 164, 255},
        .board_cell_colors = {{100, 100, 100}, {200, 200, 200}},
        .player_piece_colors = {{130, 0, 0}, {0, 0, 130}}
    },

    color_scheme_t {
        .background = {255, 246, 204},
        .buttons_background = {255, 190, 137},
        .board_cell_colors = {{153, 102, 0}, {255, 204, 102}},
        .player_piece_colors = {{0, 0, 0}, {255, 255, 255}}
    },
    
    color_scheme_t {
        .background = {255, 246, 204},
        .buttons_background = {255, 190, 137},
        .board_cell_colors = {{153, 102, 0}, {255, 204, 102}},
        .player_piece_colors = {{130, 0, 0}, {0, 0, 130}}
    },
};

#endif // CUGO_COLOR_SCHEME_H
