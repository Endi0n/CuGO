#include "board.h"
#include "point.h"
#include "list_point.h"

const point_t piece_move_directions[] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}
};

board_t* board_create(uint_t length) {
    board_t *board = new board_t;

    board->size = length;
    list_init(board->player1_pieces);
    list_init(board->player2_pieces);
    board->moves = 0;

    return board;
}

void board_delete(board_t *board) {
    list_delete(board->player1_pieces);
    list_delete(board->player2_pieces);
    delete board;
}

inline bool board_valid_point(board_t *board, point_t pos) {
    return (
        0 <= pos.x && pos.x < board->size &&
        0 <= pos.y && pos.y < board->size
    );
}

int board_group_encircled(board_t *board, list_point_t *visited) {
    list_point_t *list1 = board_current_player_pieces(board),
        *list2 = board_opponent_pieces(board);

    list_node_point_t *start_node = visited->last;

    uint_t last_length = visited->length;

    bool can_move = false;

    for (list_node_point_t *node = start_node; node; node = node->next) {
        point_t current_pos = node->value;

        for (uint_t i = 0; i < sizeof(piece_move_directions) / sizeof(point_t); ++i) {
            point_t new_pos = {current_pos.x + piece_move_directions[i].x, current_pos.y + piece_move_directions[i].y};

            if (!board_valid_point(board, new_pos) || list_find_node(start_node, new_pos)) continue;

            if (list_contains(list1, new_pos)) list_append(visited, new_pos);
            else can_move = can_move ? true : !list_contains(list2, new_pos);  // if one piece can move the entire group can
        }
    }

    return (can_move ? 0 : (visited->length - last_length + 1));
}

int board_player_defeated(board_t *board) {
    list_point_t *visited;
    list_init(visited);

    uint_t number_of_pieces_encircled = 0;

    // Check all groups of pieces
    for (list_node_point_t *node = board_current_player_pieces(board)->first; node; node = node->next) {
        if (list_contains(visited, node->value)) continue;

        list_append(visited, node->value);

        number_of_pieces_encircled += board_group_encircled(board, visited);
    }

    list_delete(visited);
    return number_of_pieces_encircled;
}

bool board_suicidal_place(board_t *board, point_t pos) {
    list_point_t *visited;
    list_init(visited);

    list_append(visited, pos);
    uint_t number_of_pieces_encircled = board_group_encircled(board, visited);

    list_delete(visited);
    return (number_of_pieces_encircled != 0);
}

bool board_place_piece(board_t *board, point_t pos) {
    if (!board_valid_point(board, pos)) return false;

    if (list_contains(board->player1_pieces, pos)
        || list_contains(board->player2_pieces, pos)
        || board_suicidal_place(board, pos)) return false;

    list_append(board_current_player_pieces(board), pos);
    board->moves++;
    return true; // valid placement
}

void board_potential_moves(board_t *board, point_t src_piece, list_point_t *potential_moves) {
    for (uint_t i = 0; i < sizeof(piece_move_directions) / sizeof(point_t); ++i) {
        point_t new_pos = {src_piece.x + piece_move_directions[i].x, src_piece.y + piece_move_directions[i].y};
        if (!board_valid_point(board, new_pos)
            || list_contains(board_current_player_pieces(board), new_pos)
            || list_contains(board_opponent_pieces(board), new_pos)) continue;
        list_append(potential_moves, new_pos);
    }
}

bool board_move_piece(board_t *board, point_t src_piece, point_t dst) {
    list_point_t *player_pieces = board_current_player_pieces(board);

    if (!list_contains(player_pieces, src_piece)) return false;

    list_point_t *potential_moves;
    list_init(potential_moves);
    board_potential_moves(board, src_piece, potential_moves);

    if(!list_contains(potential_moves, dst)) {
        list_delete(potential_moves);
        return false;
    }
    
    list_delete(potential_moves);
    list_delete(player_pieces, src_piece);
    list_append(player_pieces, dst);
    board->moves++;
    return true; // valid placement
}
