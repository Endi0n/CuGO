#ifndef CUGO_BOARD_H
#define CUGO_BOARD_H

#include "point.h"
#include "list_point.h"

enum player_e {
    PLAYER1 = 0,
    PLAYER2 = 1
};

struct board_t {
    uint_t size;
    list_point_t *player1_pieces;
    list_point_t *player2_pieces;
    uint_t moves;
};

board_t* board_create(uint_t);
void board_delete(board_t*);

inline player_e board_current_player(board_t *board) { return (player_e)(board->moves % 2); }
inline player_e board_opponent(board_t *board) { return (player_e)((board->moves + 1) % 2); }

inline list_point_t* board_player_pieces(board_t *board, player_e player)
    { return ((player == PLAYER1) ? board->player1_pieces : board->player2_pieces); }

inline list_point_t* board_current_player_pieces(board_t *board)
    { return board_player_pieces(board, board_current_player(board)); }
    
inline list_point_t* board_opponent_pieces(board_t *board)
    { return board_player_pieces(board, board_opponent(board)); }

bool board_place_piece(board_t*, point_t);
bool board_move_piece(board_t*, point_t src, point_t dst);
void board_potential_moves(board_t*, point_t, list_point_t*);

int board_player_defeated(board_t*);

#endif // CUGO_BOARD_H
