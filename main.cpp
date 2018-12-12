#include <SDL2/SDL.h>
#include "dimensions.h"
#include "board.h"
#include "render.h"
#include <iostream>
using namespace std;

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Color colors[] = {
    SDL_Color {0, 0, 0, 255},
    SDL_Color {255, 255, 255, 255}
};

board_t *board;

void init_game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = render_create_window();
    renderer = render_create_renderer(window);

    board = board_create(8);
}

void deinit_game() {
    board_delete(board);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool inside_board(int x_board, int y_board)
{
    return x_board>=0 && x_board<board->length&& y_board>=0 && y_board<board->length;
}

void get_board_coord(int x_screen, int y_screen, int &x_board, int &y_board)
{
    x_screen=x_screen-BOARD_OFFSET_X;
    x_board=x_screen/BOARD_CELL_SIZE;
    y_screen=y_screen-BOARD_OFFSET_Y;
    y_board=y_screen/BOARD_CELL_SIZE;

}

void handle_mouse_click(SDL_MouseButtonEvent &mouse) {
        if(true)
        {
            int x=mouse.x, y=mouse.y;
            int x_board, y_board;
            get_board_coord(x, y, x_board, y_board);

            list_point_t *list = (board->moves % 2 == 0) ? board->player1_pieces : board->player2_pieces;
            point_t pos = {x_board, y_board};
            list_append(list, pos);
            board->moves++;
        }
}

int main(int argv, char **args) {
    init_game();
    point_t pos = {0, 0}, pos2={1,1};
    list_append(board->player1_pieces, pos);
    list_append(board->player2_pieces, pos2);

    SDL_Event window_event;
    bool exit_game_loop = false;

    do {
        render_clear(renderer, {255, 255, 204, 255});
        render_board(renderer, board, colors);

        SDL_WaitEvent(&window_event);
        switch (window_event.type) {
            case SDL_QUIT:
                exit_game_loop = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                handle_mouse_click(window_event.button);
                break;
        }

        SDL_RenderPresent(renderer);
    } while (!exit_game_loop);

    deinit_game();
    return 0;
}
