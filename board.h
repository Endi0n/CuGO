#ifndef CUGO_BOARD_H
#define CUGO_BOARD_H

typedef unsigned int uint_t;

enum piece_e {
    EMPTY = 0,
    PLAYER1 = 1,
    PLAYER2 = 2
};

struct pos_t {
    uint_t x;
    uint_t y;
};

struct board_t {
    piece_e **piece;
    uint_t length;
    uint_t moves;
};

board_t* board_create(uint_t);

void board_delete(board_t*);

void board_place_piece(board_t*, pos_t, piece_e);

void board_move_piece(board_t*, pos_t, pos_t);

void board_render(board_t);

#endif // CUGO_BOARD_H
