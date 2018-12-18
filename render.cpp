#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "dimensions.h"
#include "board.h"
#include "list_point.h"

SDL_Window* render_create_window() {
    return SDL_CreateWindow(
        "CuGO Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI
    );
}

SDL_Renderer* render_create_renderer(SDL_Window *window) {
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    return renderer;
}

void SDL_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void render_clear(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color);
    SDL_RenderClear(renderer);
}

void render_text(SDL_Renderer *renderer, const char *text, uint_t size, point_t pos, SDL_Color color) {
    TTF_Font *font = TTF_OpenFont("sans.ttf", size);
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {.x = pos.x, .y = pos.y};
    TTF_SizeText(font, text, &rect.w, &rect.h);

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void render_logo(SDL_Renderer *renderer) {
    render_text(
        renderer,
        "CuGO",
        LOGO_SIZE,
        {LOGO_OFFSET_X, LOGO_OFFSET_Y},
        {0, 0, 0} // Render with black color 
    );
}

void render_filled_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_Point points[] = {
            {cx + x, cy + y},
            {cx - x, cy + y},

            {cx + y, cy + x},
            {cx - y, cy + x},

            {cx - x, cy - y},
            {cx + x, cy - y},

            {cx - y, cy - x},
            {cx + y, cy - x}
        };

        SDL_RenderDrawLines(renderer, points, sizeof(points) / sizeof(SDL_Point));

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void render_board_cell(SDL_Renderer *renderer, int x, int y, SDL_Color color) {
    // Ot
    SDL_SetRenderDrawColor(renderer, color);

    SDL_Rect rect = {
        BOARD_OFFSET_X + x * BOARD_CELL_SIZE,
        BOARD_OFFSET_Y + y * BOARD_CELL_SIZE,
        BOARD_CELL_SIZE,
        BOARD_CELL_SIZE
    };
    
    SDL_RenderFillRect(renderer, &rect);
}

void render_board_grid(SDL_Renderer *renderer, board_t *board) {
    // Ot
    static const SDL_Color color1 = {255, 204, 102}, color2 = {153, 102, 0};

    for(uint_t x = 0; x < board->length; ++x)
        for(uint_t y = 0; y < board->length; ++y)
            render_board_cell(renderer, x, y, ((x + y) % 2 == 0) ? color1 : color2);
}

void render_board_piece(SDL_Renderer *renderer, point_t piece) {
    // Ot
    render_filled_circle(
        renderer,
        BOARD_OFFSET_X + BOARD_CELL_SIZE * piece.x + BOARD_CELL_SIZE / 2,
        BOARD_OFFSET_Y + BOARD_CELL_SIZE * piece.y + BOARD_CELL_SIZE / 2,
        BOARD_CELL_SIZE / 3
    );
}

void render_board_pieces(SDL_Renderer *renderer, list_point_t *pieces, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color);
    for (list_node_point_t *node = pieces->first; node; node = node->next)
        render_board_piece(renderer, node->value);
}

void render_board_potential_piece(SDL_Renderer *renderer, point_t piece) {
    render_filled_circle(
        renderer,
        BOARD_OFFSET_X + BOARD_CELL_SIZE * piece.x + BOARD_CELL_SIZE / 2,
        BOARD_OFFSET_Y + BOARD_CELL_SIZE * piece.y + BOARD_CELL_SIZE / 2,
        BOARD_CELL_SIZE / 10
    );
}

void render_board_piece_selector(SDL_Renderer *renderer, board_t *board, point_t piece, const SDL_Color colors[2]) {
    SDL_SetRenderDrawColor(renderer, colors[board_current_player(board)]);

    list_point_t *potential_moves;
    list_init(potential_moves);
    board_potential_moves(board, piece, potential_moves);

    for (list_node_point_t *node = potential_moves->first; node; node = node->next)
        render_board_potential_piece(renderer, node->value);

    list_delete(potential_moves);
}

void render_board(SDL_Renderer* renderer, board_t *board, const SDL_Color colors[2]) {
    render_board_grid(renderer, board);
    render_board_pieces(renderer, board->player1_pieces, colors[0]);
    render_board_pieces(renderer, board->player2_pieces, colors[1]);
}

void render_turn_info(SDL_Renderer *renderer, board_t *board, const SDL_Color colors[2]) {
    static const char *place = "has to place";
    static const char *move = "has to move";
    const char *msg = (board->moves < 16) ? place : move;
    
    SDL_SetRenderDrawColor(renderer, colors[board_current_player(board)]);
    render_filled_circle(renderer,  30,  30, 12);
    render_text(
        renderer,
        msg,
        14,
        {50, 20},
        {0, 0, 0, 0} // Render with black color 
    );

    render_text(
        renderer,
        "No. of moves: ",
        14,
        {WINDOW_WIDTH - 150, 20},
        {0, 0, 0, 0}
    );

    char moves[5] = {
        (char)((board->moves / 1000) % 10 + '0'),
        (char)((board->moves / 100) % 10 + '0'),
        (char)((board->moves / 10) % 10 + '0'),
        (char)(board->moves % 10 + '0'),
        '\0'
    };

    render_text(
        renderer,
        moves,
        14,
        {WINDOW_WIDTH - 55, 20},
        {0, 0, 0, 0} // Render with black color 
    );
}
