#include <board.h>

void init_board(board &board, uint_t length) {
    // O
    board.moves = 0;
    board.length = length;
    board.board = new *element_t[length];
    
    for (auto i = 0; i < board.length; i++)
        board.board[i] = new element_t[length];
}