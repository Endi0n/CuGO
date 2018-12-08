#include "board.h"
#include "list_point.h"

board_t* board_create(uint_t length) {
    // Oc
    board_t *board = new board_t;

    board->length = length;
    list_init(board->player1_pieces);
    list_init(board->player2_pieces);
    board->moves = 0;

    return board;
}

void board_delete(board_t* board) {
    // Oc
    list_delete(board->player1_pieces);
    list_delete(board->player2_pieces);
    delete board;
}
