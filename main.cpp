#include <SDL2/SDL.h>
#include "dimensions.h"
#include "board.h"
#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Color colors[] = {
    SDL_Color {255, 255, 255, 255},
    SDL_Color {255, 0, 0, 255},
    SDL_Color {0, 0, 255, 255}
};

board_t *board;

void init_game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = render_create_window();
    renderer = render_create_renderer(window);

    board = board_create(8);
}

void deinit_game() {
    board_delete(board);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handle_mouse_click(SDL_MouseButtonEvent& mouse);

int main() {
    init_game();

    SDL_Event window_event;
    bool exit_game_loop = false;

    render_clear(renderer);
    SDL_RenderPresent(renderer);

    do {
        render_clear(renderer, {230, 230, 230, 255});
        render_board(renderer, board, colors);

        SDL_WaitEvent(&window_event);
        switch (window_event.type) {
            case SDL_QUIT:
                exit_game_loop = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                // handle_mouse_click(window_event.button);
                break;
        }

        SDL_RenderPresent(renderer);
    } while (!exit_game_loop);

    deinit_game();
    return 0;
}
