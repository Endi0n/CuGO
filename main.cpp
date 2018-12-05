#include <SDL2/SDL.h>
#include "board.h"

const int WIDTH = 800, HEIGHT = 600;

SDL_Window* create_game_window() {
    // Oc
    SDL_Init(SDL_INIT_EVERYTHING);

    return SDL_CreateWindow(
        "CuGO Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI 
    );
}

SDL_Renderer* create_game_renderer(SDL_Window *window) {
    // Oc
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    return renderer;
}

int main() {
    // Oc
    SDL_Window *window = create_game_window();
    SDL_Renderer *renderer = create_game_renderer(window); 
    SDL_Event windowEvent;

    bool exit_game_loop = false;

    board_t *board = board_create(8);

    do {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // board_render(board);

        SDL_RenderPresent(renderer);

        SDL_WaitEvent(&windowEvent);
        if (windowEvent.type == SDL_QUIT)
            exit_game_loop = true;
    } while (!exit_game_loop);

    board_delete(board);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
