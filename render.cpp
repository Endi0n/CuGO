#include <SDL2/SDL.h>
#include "dimensions.h"
#include "board.h"
#include "list_point.h"

SDL_Window* render_create_window() {
    return SDL_CreateWindow(
        "CuGO Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI 
    );
}

SDL_Renderer* render_create_renderer(SDL_Window *window) {
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    return renderer;
}

void SDL_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void render_clear(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color);
    SDL_RenderClear(renderer);
}

void render_filled_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_Point points[] = {
            {cx + x, cy + y},
            {cx - x, cy + y},

            {cx + y, cy + x},
            {cx - y, cy + x},
            
            {cx - x, cy - y},
            {cx + x, cy - y},

            {cx - y, cy - x},
            {cx + y, cy - x}
        };

        SDL_RenderDrawLines(renderer, points, sizeof(points) / sizeof(SDL_Point));

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void render_board_grid(SDL_Renderer*, board_t*, SDL_Color);

void render_board_piece(SDL_Renderer*, point_t);

void render_board_pieces(SDL_Renderer*, list_point_t*, SDL_Color);

void render_board(SDL_Renderer* renderer, board_t *board, const SDL_Color colors[3]) {
    // board_draw_grid(renderer, board, colors[0]);
    // board_draw_pieces(renderer, board->player1_pieces, colors[1]);
    // board_draw_pieces(renderer, board->player2_pieces, colors[2]);
}
