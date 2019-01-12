#include <SDL2/SDL.h>
#include "dimensions.h"
#include "board.h"
#include "menu.h"
#include "render.h"
#include "sound.h"

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

SDL_Rect menu_btn = {WINDOW_WIDTH - 120, 550, 100, 30};

void handle_mouse_click(SDL_MouseButtonEvent &mouse) {
    if (menu_button_pressed(mouse, menu_btn)) {
        menu_visible(true);
        return;
    }

    if (game_over) return;

    if (board->moves < board->size * 2) {
        // Initial placing
        if (board_place_piece(board, board_position(mouse)))
            sound_play_place_piece();
    } else {
        // Moves
        if (!piece_selected) {
            point_t pos = board_position(mouse);
            if (list_contains(board_current_player_pieces(board), pos)) {
                piece = pos;
                piece_selected = true;
            }
            return;
        }
        
        piece_selected = false;
        
        if(board_move_piece(board, piece, board_position(mouse)))
             sound_play_place_piece();
    }

    if (int aux = board_player_defeated(board)) {
        game_over = true;
        sound_play_tada();
    }
}

void render_turn_info(board_t *board, const color_scheme_t &color_scheme) {
    const char *const place = "has to place";
    const char *const move = "has to move";
    const char *const won = "WON";

    const char *msg = (!game_over) ? (board->moves < board->size * 2) ? place : move : won;

    int player = (!game_over) ? board_current_player(board) : board_opponent(board);
    
    render_circle(30,  30, 12, color_scheme.player_piece_colors[player]);
    render_text(msg, 14, {50, 20}, {0, 0, 0, 0});

    render_text("No. of moves: ", 14, {WINDOW_WIDTH - 150, 20}, {0, 0, 0, 0});

    char moves[5] = {
        (char)((board->moves / 1000) % 10 + '0'),
        (char)((board->moves / 100) % 10 + '0'),
        (char)((board->moves / 10) % 10 + '0'),
        (char)(board->moves % 10 + '0'),
        '\0'
    };

    render_text(moves, 14, {WINDOW_WIDTH - 55, 20}, {0, 0, 0, 0});
}

void game_loop(SDL_Event window_event) {
    render_clear({204, 223, 255});
    render_logo();

    render_board(board, menu_color_scheme());

    render_button(menu_btn, "Menu", {137, 164, 255}, {255, 255, 255});

    if (piece_selected) render_board_piece_selector(board, piece, menu_color_scheme());

    render_turn_info(board, menu_color_scheme());

    if (window_event.type == SDL_MOUSEBUTTONDOWN)
        handle_mouse_click(window_event.button);

    render_present();
}
