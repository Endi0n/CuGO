#ifndef CUGO_RENDER_H
#define CUGO_RENDER_H

#include <SDL2/SDL.h>
#include "board.h"
#include "color_scheme.h"

void render_init();
void render_clear(SDL_Color color);
void render_present();
void render_rect(const SDL_Rect &rect, SDL_Color color);
void render_circle(int radius, point_t center, SDL_Color color);
void render_text(const char *text, uint_t size, point_t position, SDL_Color color);
void render_button(const SDL_Rect &button, const char *text, SDL_Color background_color, SDL_Color color);
void render_logo();
void render_board_piece_selector(board_t *board, point_t offset, point_t piece, const color_scheme_t &color_scheme);
void render_board(board_t *board, point_t offset, const color_scheme_t &color_scheme);
void render_deinit();

#endif // CUGO_RENDER_H
