#include "dimensions.h"
#include "board.h"
#include "menu.h"
#include "render.h"

#include <iostream>
using namespace std;

board_t *board;
bool game_over = true;

void game_init() {
    game_over = false;
    board = board_create(menu_board_size());
}

void game_deinit() {
    game_over = true;
    board_delete(board);
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

void game_loop(SDL_Event window_event) {
    render_clear({204, 223, 255});
    render_logo();

    render_board(board, menu_color_scheme());

    if (piece_selected) render_board_piece_selector(board, piece, menu_color_scheme());

    render_turn_info(board, menu_color_scheme());

    if (window_event.type == SDL_MOUSEBUTTONDOWN)
        handle_mouse_click(window_event.button);

    render_present();
}
