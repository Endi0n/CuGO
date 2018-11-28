#include <board.h>

void init_board(board &board, uint_t length) {
    // Oc
    board.moves = 0;
    board.length = length;
    board.board = new *element_t[length];

    for (auto i = 0; i < board.length; i++)
        board.board[i] = new element_t[length];
}

void place_element_board(board_t board, pos_t p, element_t e) {
    board.board[p.x][p.y]=e;
    board.moves++;
}

void move_element_board(board_t& board, pos_t p1, pos_t p2) {
    place_element_board(board, p1, board.board[p2.x][p2.y]);
    place_element_board(board, p2, element_t.EMPTY);
    board.moves++;
}
