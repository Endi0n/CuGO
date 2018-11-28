#ifndef CUGO_BOARD_H
#define CUGO_BOARD_H

#include <stdint.h>

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

board_t* create_board(uint_t);

void delete_board(board_t*);

void place_piece_board(board_t*, pos_t, piece_e);

void move_piece_board(board_t*, pos_t, pos_t);

void render_board(board_t);

#endif // CUGO_BOARD_H
