#include <SDL2/SDL.h>
#include "dimensions.h"
#include "board.h"
#include "render.h"

#include <iostream>
using namespace std;

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Color colors[] = {
    SDL_Color {0, 0, 0, 255},
    SDL_Color {255, 255, 255, 255}
};

board_t *board;

bool game_over = false;

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

void handle_mouse_click(SDL_MouseButtonEvent &mouse) {
    // Ot
    if (game_over) return;

    if (mouse.button != SDL_BUTTON_LEFT) return;

    if (board->moves < board->length * 2) {
        // Initial placing
        board_place_piece(board, board_current_player(board), {
            // (Relative position from board offset / Cell size) = Position of the click in the 0..<board.length grid
            (mouse.x - BOARD_OFFSET_X) / BOARD_CELL_SIZE,
            (mouse.y -BOARD_OFFSET_Y) / BOARD_CELL_SIZE
        });
    } else {
        // Moves 
    }

    if (int aux = board_player_defeated(board, board_current_player(board))) {
        cout << "Player " << board_current_player(board) << " was defeated. Score for opponent " << aux << '.' << endl;
        game_over = true;
    }
}

int main(int argv, char **args) {
    init_game();

    SDL_Event window_event;
    bool exit_game_loop = false;

    do {
        render_clear(renderer, {255, 255, 204, 255});
        render_board(renderer, board, colors);

        SDL_WaitEvent(&window_event);
        switch (window_event.type) {
            case SDL_QUIT:
                exit_game_loop = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                handle_mouse_click(window_event.button);
                break;
        }

        SDL_RenderPresent(renderer);
    } while (!exit_game_loop);

    deinit_game();
    return 0;
}
