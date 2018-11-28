#include "board.h"

board_t* create_board(uint_t length) {
    board_t *board = new board_t;

    board->moves = 0;
    board->length = length;
    board->piece = new piece_e*[length];

    for (uint_t i = 0; i < length; i++)
        board->piece[i] = new piece_e[length];
    
    return board;
}

void delete_board(board_t* board) {
    for (uint_t i = 0; i < board->length; i++)
        delete[] board->piece[i];
    delete[] board->piece;
    delete board;
}

void place_piece_board(board_t *board, pos_t p, piece_e e) {
    // Ot
    board->piece[p.x][p.y] = e;
    board->moves++;
}

void move_piece_board(board_t *board, pos_t p1, pos_t p2) {
    // Ot
    place_piece_board(board, p1, board->piece[p2.x][p2.y]);
    place_piece_board(board, p2, EMPTY);
    board->moves++;
}

void draw_board_lines(board_t*);

void draw_board_piece(pos_t, piece_e);

void draw_board_pieces(board_t*);

void render_board(board_t *board) {
    // draw_board_lines(board);
    // draw_board_pieces(board);
}
