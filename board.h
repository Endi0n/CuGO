#include <stdint.h>

struct board_t {
    element_t **board;
    uint_t length;
    uint_t moves;
};

enum element_t {
    EMPTY = 0,
    PLAYER1 = 1,
    PLAYER2 = 2
}

struct pos_t {
    uint_t x, y;
};

void init_board(board_t&, uint_t);

void place_element_board(board_t&, pos_t, element_t);

void move_element_board(board_t&, pos_t, pos_t);

void render_board(board_t);

