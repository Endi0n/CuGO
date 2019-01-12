#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "dimensions.h"
#include "color_scheme.h"
#include "board.h"
#include "list_point.h"

void SDL_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

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

SDL_Window *window;
SDL_Renderer *renderer;

void render_init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window = render_create_window();
    renderer = render_create_renderer(window);
}

void render_deinit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void render_clear(SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color);
    SDL_RenderClear(renderer);
}

void render_present() {
    SDL_RenderPresent(renderer);
}

void render_rect(const SDL_Rect &rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color);
    SDL_RenderFillRect(renderer, &rect);
}

void render_circle(int cx, int cy, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color);

    int x = radius - 1;
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

void render_text(const char *text, uint_t size, point_t pos, SDL_Color color) {
    TTF_Font *font = TTF_OpenFont("sans.ttf", size);
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {.x = pos.x, .y = pos.y};
    TTF_SizeText(font, text, &rect.w, &rect.h);

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void render_logo() {
    render_text(
        "CuGO",
        LOGO_SIZE,
        {LOGO_OFFSET_X, LOGO_OFFSET_Y},
        {0, 0, 0} // Render with black color 
    );
}

void render_board_cell(int x, int y, SDL_Color color) {
    SDL_Rect rect = {
        BOARD_OFFSET_X + x * BOARD_CELL_SIZE,
        BOARD_OFFSET_Y + y * BOARD_CELL_SIZE,
        BOARD_CELL_SIZE,
        BOARD_CELL_SIZE
    };
    
    render_rect(rect, color);
}

void render_board_grid(board_t *board, const color_scheme_t &color_scheme) {
    for(uint_t x = 0; x < board->size; ++x)
        for(uint_t y = 0; y < board->size; ++y)
            render_board_cell(x, y, color_scheme.board_cell_colors[(x + y) % 2]);
}

void render_board_piece(point_t piece, SDL_Color color) {
    render_circle(
        BOARD_OFFSET_X + BOARD_CELL_SIZE * piece.x + BOARD_CELL_SIZE / 2,
        BOARD_OFFSET_Y + BOARD_CELL_SIZE * piece.y + BOARD_CELL_SIZE / 2,
        BOARD_CELL_SIZE / 3,
        color
    );
}

void render_board_pieces(list_point_t *pieces, SDL_Color color) {
    for (list_node_point_t *node = pieces->first; node; node = node->next)
        render_board_piece(node->value, color);
}

void render_board_potential_piece(point_t piece, SDL_Color color) {
    render_circle(
        BOARD_OFFSET_X + BOARD_CELL_SIZE * piece.x + BOARD_CELL_SIZE / 2,
        BOARD_OFFSET_Y + BOARD_CELL_SIZE * piece.y + BOARD_CELL_SIZE / 2,
        BOARD_CELL_SIZE / 10,
        color
    );
}

void render_board_piece_selector(board_t *board, point_t piece, const color_scheme_t &color_scheme) {
    SDL_Color color = color_scheme.player_piece_colors[board_current_player(board)];

    list_point_t *potential_moves;
    list_init(potential_moves);
    board_potential_moves(board, piece, potential_moves);

    for (list_node_point_t *node = potential_moves->first; node; node = node->next)
        render_board_potential_piece(node->value, color);

    list_delete(potential_moves);
}

void render_board(board_t *board, const color_scheme_t &color_scheme) {
    render_board_grid(board, color_scheme);
    render_board_pieces(board->player1_pieces, color_scheme.player_piece_colors[0]);
    render_board_pieces(board->player2_pieces, color_scheme.player_piece_colors[1]);
}

void render_button(const SDL_Rect &rect, const char *text, SDL_Color button_color, SDL_Color text_color) {
    int text_width, text_height;
    
    TTF_Font *font = TTF_OpenFont("sans.ttf", 16);
    TTF_SizeText(font, text, &text_width, &text_height);
    TTF_CloseFont(font);

    render_rect(rect, button_color);
    render_text(text, 16, {rect.x + (rect.w - text_width) / 2, rect.y + (rect.h - text_height) / 2}, text_color);
}
