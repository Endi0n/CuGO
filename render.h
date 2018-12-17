#ifndef CUGO_RENDER_H
#define CUGO_RENDER_H

#include <SDL2/SDL.h>
#include "board.h"

SDL_Window* render_create_window();
SDL_Renderer* render_create_renderer(SDL_Window*);
void SDL_SetRenderDrawColor(SDL_Renderer*, SDL_Color);

void render_clear(SDL_Renderer*, SDL_Color);
void render_text(SDL_Renderer*, char*, uint_t, SDL_Rect, SDL_Color);
void render_logo(SDL_Renderer*);
void render_board(SDL_Renderer*, board_t*, const SDL_Color[2]);

#endif // CUGO_RENDER_H
