#ifndef CUGO_RENDER_H
#define CUGO_RENDER_H

#include <SDL2/SDL.h>
#include "point.h"
#include "board.h"
#include "color_scheme.h"

void render_init();
void render_clear(SDL_Color);
void render_present();
void render_rect(const SDL_Rect&, SDL_Color);
void render_circle(int, point_t, SDL_Color);
void render_text(const char*, uint_t, point_t, SDL_Color);
void render_button(const SDL_Rect&, const char*, SDL_Color, SDL_Color);
void render_logo();
void render_board_piece_selector(board_t*, point_t, point_t, const color_scheme_t&);
void render_board(board_t*, point_t, const color_scheme_t&);
void render_deinit();

#endif // CUGO_RENDER_H
