#include "menu.h"

color_scheme_t color_schemes[] = {
    color_scheme_t {
        .board_cell_colors = {{100, 100, 100}, {200, 200, 200}},
        .player_piece_colors = {{0, 0, 0}, {255, 255, 255}}
    },
    color_scheme_t {
        .board_cell_colors = {{255, 204, 102}, {153, 102, 0}},
        .player_piece_colors = {{0, 0, 0}, {255, 255, 255}}
    }
};

color_scheme_t color_scheme = color_schemes[0];

bool menu_visible() { return false; }

uint menu_board_size() { return 8; }

color_scheme_t menu_color_scheme() { return color_scheme; }

void menu_loop(SDL_Event) {}
