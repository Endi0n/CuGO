#include <board.h>

int main() {
    board_t board;
    
    init_board(board);
    render_board(board);

    return 0;
}