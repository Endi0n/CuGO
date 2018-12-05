#include "board.h"

board_t* board_create(uint_t length) {
    board_t *board = new board_t;

    board->moves = 0;
    board->length = length;
    board->piece = new piece_e*[length];

    for (uint_t i = 0; i < length; i++)
        board->piece[i] = new piece_e[length];
    
    return board;
}

void board_delete(board_t* board) {
    for (uint_t i = 0; i < board->length; i++)
        delete[] board->piece[i];
    delete[] board->piece;
    delete board;
}

void board_place_piece(board_t *board, pos_t p, piece_e e) {
    // Ot
    board->piece[p.x][p.y] = e;
    board->moves++;
}

void board_move_piece(board_t *board, pos_t p1, pos_t p2) {
    // Ot
    board_place_piece(board, p1, board->piece[p2.x][p2.y]);
    board_place_piece(board, p2, EMPTY);
    board->moves++;
}

void board_draw_lines(board_t*);

void board_draw_piece(pos_t, piece_e);

void board_draw_pieces(board_t*);

void board_render(board_t *board) {
    // board_draw_lines(board);
    // board_draw_pieces(board);
}
