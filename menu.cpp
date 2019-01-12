#include <SDL2/SDL.h>
#include "menu.h"
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

SDL_Rect play_btn = {200,150,400,50};
SDL_Rect rules_btn = {200,230,400,50};
SDL_Rect back_btn = {200, 500, 400, 50};

bool show_rules = false;

bool menu_button_pressed(SDL_MouseButtonEvent &mouse, SDL_Rect button) {
    return (
        mouse.x >= button.x && mouse.x <= button.x + button.w
        && mouse.y >= button.y && mouse.y <= button.y + button.h
    );
}

void menu_handle_mouse_click(SDL_MouseButtonEvent &mouse) {
    if (show_rules) {
        if(menu_button_pressed(mouse, back_btn))
            show_rules = false;
        return;
    }

    if(menu_button_pressed(mouse, rules_btn))
        show_rules = true;
    
    if(menu_button_pressed(mouse, play_btn))
        menu_visible(false);
}

void render_menu() {
    render_button(play_btn, "Play", {137, 164, 255}, {255, 255, 255});
    render_button(rules_btn, "Rules", {137, 164, 255}, {255, 255, 255});
}

void render_rules() {
    SDL_Rect bg = {200, 120, 400, 350};
    render_rect(bg, {255, 255, 255});
    render_text("Game Rules:", 16, {210, 130}, {0,0,0});

    const char *const rules[] = {
        "Each player has 8 pieces.",
    };

    for (int i = 0; i < sizeof(rules) / sizeof(char*); i++) {
        render_circle(215, 167 + 18 * i, 3, {0, 0, 0});
        render_text(rules[i], 14, {225, 157 + 18 * i}, {0, 0, 0});
    }

    render_button(back_btn, "Back", {137, 164, 255}, {255, 255, 255});
}

void menu_loop(SDL_Event window_event) {
    render_clear({204, 223, 255});
    render_logo();

    if (show_rules) render_rules();
    else render_menu();

    if (window_event.type == SDL_MOUSEBUTTONDOWN)
        menu_handle_mouse_click(window_event.button);
    
    render_present();
}
