#include "board.h"
#include "point.h"
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

void board_delete(board_t *board) {
    // Oc
    list_delete(board->player1_pieces);
    list_delete(board->player2_pieces);
    delete board;
}

bool board_point_valid(board_t *board, point_t pos) {
    return (
        0 <= pos.x && pos.x < board->length &&
        0 <= pos.y && pos.y < board->length
    );
}

int board_player_defeated(board_t *board, player_e player) {
    // Oc
    static point_t directions[] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    list_point_t *visited;
    list_init(visited);

    list_point_t *list1 = board->player1_pieces, *list2 = board->player2_pieces;    
    if (player == PLAYER2) {
        list_point_t *aux = list1;
        list1 = list2;
        list2 = aux;
    }

    // Check all groups of pieces
    for (list_node_point_t *node = list1->first; node; node = node->next) {
        if (list_contains(visited, node->value)) continue;

        uint_t last_length = visited->length;
        list_append(visited, node->value);

        list_node_point_t *start_node = visited->last;
        bool can_move = false;

        // Check a group of pieces
        for (list_node_point_t *node = start_node; node; node = node->next) {
            point_t current_pos = node->value;

            for (uint_t i = 0; i < sizeof(directions) / sizeof(point_t); ++i) {
                point_t new_pos = {current_pos.x + directions[i].x, current_pos.y + directions[i].y};

                if (!board_point_valid(board, new_pos) || list_find_node(start_node->next, new_pos)) continue;

                if (list_contains(list1, new_pos)) list_append(visited, new_pos);
                else can_move = !list_contains(list2, new_pos);
            }
        }

        // It's enough for one group of pieces to be encircled in order for a player to be defeated
        if (!can_move) {
            uint_t current_length = visited->length;
            list_delete(visited);
            return (current_length - last_length);
        }
    }
    list_delete(visited);
    return 0;
}
