#include <SDL2/SDL.h>
#include "menu.h"
#include "dimensions.h"
#include "game.h"
#include "render.h"

color_scheme_t color_schemes[] = {
    color_scheme_t {
        .board_cell_colors = {{100, 100, 100}, {200, 200, 200}},
        .player_piece_colors = {{0, 0, 0}, {255, 255, 255}}
    },
    color_scheme_t {
        .board_cell_colors = {{255, 204, 102}, {153, 102, 0}},
        .player_piece_colors = {{0, 0, 0}, {255, 255, 255}}
    }
};

color_scheme_t color_scheme = color_schemes[0];

bool menu_show = true;
bool menu_visible() { return menu_show; }
bool menu_visible(bool visible) { menu_show = visible; return visible; }

uint menu_board_size() { return 8; }

color_scheme_t menu_color_scheme() { return color_scheme; }

const int MENU_BUTTON_OFFSET_X = (WINDOW_WIDTH - 400) / 2;

SDL_Rect play_btn = {MENU_BUTTON_OFFSET_X, 150, 400, 50};
SDL_Rect customize_btn = {MENU_BUTTON_OFFSET_X, 230, 400, 50};
SDL_Rect rules_btn = {MENU_BUTTON_OFFSET_X, 310, 400, 50};
SDL_Rect reset_btn = {MENU_BUTTON_OFFSET_X, 390, 400, 50};
SDL_Rect back_btn = {MENU_BUTTON_OFFSET_X, 500, 400, 50};

enum menu_state_e {
    DEFAULT,
    CUSTOMIZE,
    RULES,
};

menu_state_e menu_state = DEFAULT;

bool game_inited = false;

bool menu_button_pressed(SDL_MouseButtonEvent mouse, SDL_Rect button) {
    return (
        mouse.x >= button.x && mouse.x <= button.x + button.w
        && mouse.y >= button.y && mouse.y <= button.y + button.h
    );
}

void menu_handle_mouse_click(SDL_MouseButtonEvent mouse) {
    if (menu_state != DEFAULT) {
        if(menu_button_pressed(mouse, back_btn))
            menu_state = DEFAULT;
        return;
    }

    if (menu_button_pressed(mouse, customize_btn)) {
        menu_state = CUSTOMIZE;
        return;
    }

    if(menu_button_pressed(mouse, rules_btn)) {
        menu_state = RULES;
        return;
    }
    
    if(menu_button_pressed(mouse, play_btn)) {
        if (!game_inited) {
            game_inited = true;
            game_init();
        }
        menu_visible(false);
        return;
    }

    if (game_started() && menu_button_pressed(mouse, reset_btn)) {
        game_deinit();
        game_inited = false;
        return;
    }
}

void render_menu() {
    SDL_Color btn_bg = {137, 164, 255}, btn_color = {255, 255, 255};

    const char *play = (game_started()) ? "Resume" : "Play";
    render_button(play_btn, play, btn_bg, btn_color);

    render_button(customize_btn, "Customize", btn_bg, btn_color);
    render_button(rules_btn, "Rules", btn_bg, btn_color);

    if (game_started())
        render_button(reset_btn,  "Reset", btn_bg, btn_color);
}

void render_customize() {

}

void render_rules() {
    const int width = 550;
    const int offset_x = (WINDOW_WIDTH - width) / 2;

    SDL_Rect bg = {offset_x, 120, width, 350};
    render_rect(bg, {255, 255, 255});
    render_text("Game Rules:", 16, {offset_x + 10, 130}, {0,0,0});

    const char *const rules[] = {
        "Each player has a limited amount of pieces, equal to the size of the board.",
        "The objective is to encircle at least one of the opponent's pieces.",
        "Players begin by placing their pieces.",
        "After all the pieces have been placed, players can move their pieces.",
        "The pieces can be moved up, down, left, or right.",
    };

    for (int i = 0; i < sizeof(rules) / sizeof(char*); i++) {
        render_circle(offset_x + 15, 167 + 18 * i, 3, {0, 0, 0});
        render_text(rules[i], 14, {offset_x + 25, 157 + 18 * i}, {0, 0, 0});
    }
}

void menu_loop(SDL_Event window_event) {
    render_clear({204, 223, 255});
    render_logo();

    switch (menu_state) {
    case CUSTOMIZE:
        render_customize();
        break;
    case RULES:
        render_rules();
        break;
    default:
        render_menu();
        break;
    }

    if (menu_state != DEFAULT)
        render_button(back_btn, "Back", {137, 164, 255}, {255, 255, 255});

    if (window_event.type == SDL_MOUSEBUTTONDOWN)
        menu_handle_mouse_click(window_event.button);
    
    render_present();
}
