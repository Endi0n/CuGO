#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "dimensions.h"
#include "color_scheme.h"
#include "render.h"
#include "board.h"
#include "list_point.h"

#include <iostream>
using namespace std;

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

board_t *board;
bool game_over = false;

void init_game() {
    board = board_create(8);
    render_init();
}

void deinit_game() {
    board_delete(board);
    render_deinit();
}

point_t board_position(SDL_MouseButtonEvent &mouse) {
    // Ot
    return {
        // (Relative position from board offset / Cell size) = Position of the click in the 0..<board.size grid
        (mouse.x - BOARD_OFFSET_X) / BOARD_CELL_SIZE,
        (mouse.y - BOARD_OFFSET_Y) / BOARD_CELL_SIZE
    };
}

point_t piece;
bool piece_selected = false;

void handle_mouse_click(SDL_MouseButtonEvent &mouse) {
    if (game_over) return;

    if (mouse.button != SDL_BUTTON_LEFT) return;

    if (board->moves < board->size * 2) {
        // Initial placing
        board_place_piece(board, board_position(mouse));
    } else {
        // Moves
        if (piece_selected) {
            piece_selected = false;
            board_move_piece(board, piece, board_position(mouse));
        } else {
            point_t pos = board_position(mouse);
            if (list_contains(board_current_player_pieces(board), pos)) {
                piece = pos;
                piece_selected = true;
            }
        }
    }

    if (int aux = board_player_defeated(board)) {
        cout << "Player " << board_current_player(board) << " was defeated. Score for opponent " << aux << '.' << endl;
        game_over = true;
    }
}

int main(int argv, char **args) {
    init_game();

    SDL_Event window_event;
    bool exit_game_loop = false;

    do {
        render_clear({204, 223, 255});
        render_logo();

        render_board(board, color_scheme);

        if (piece_selected) render_board_piece_selector(board, piece, color_scheme);

        render_turn_info(board, color_scheme);

        SDL_WaitEvent(&window_event);
        switch (window_event.type) {
            case SDL_QUIT:
                exit_game_loop = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                handle_mouse_click(window_event.button);
                break;
        }

        render_present();
    } while (!exit_game_loop);

    deinit_game();
    return 0;
}
