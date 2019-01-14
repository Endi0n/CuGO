#include <SDL2/SDL.h>
#include "menu.h"
#include "dimensions.h"
#include "color_scheme.h"
#include "game.h"
#include "render.h"

// Menu visibility getter and setter
bool menu_show = true;
bool menu_visible() { return menu_show; }
bool menu_visible(bool visible) { menu_show = visible; return visible; }

// Color scheme getter and setter
int color_scheme_id = 0;
color_scheme_t color_scheme = color_schemes[0];
color_scheme_t menu_color_scheme() { return color_scheme; }

// Sound active getter and setter
bool sound = true;
bool menu_sound() { return sound; }

// Board size getter and setter
int board_size = 8;
uint_t menu_board_size() { return board_size; }

// Menu state 
enum menu_state_e {
    DEFAULT,
    CUSTOMIZE,
    RULES,
};

menu_state_e menu_state = DEFAULT;

// Main buttons
const SDL_Rect play_btn = {200, 150, 400, 50};
const SDL_Rect customize_btn = {200, 230, 400, 50};
const SDL_Rect rules_btn = {200, 310, 400, 50};
const SDL_Rect reset_btn = {200, 390, 400, 50};

// Customization menu buttons
const SDL_Rect size_l_btn = {415, 152, 20, 25};
const SDL_Rect size_m_btn = {485, 152, 20, 25};
const SDL_Rect sound_btn = {446, 201, 30, 25};
const SDL_Rect theme_l_btn = {438, 252, 20, 25};
const SDL_Rect theme_r_btn = {463, 252, 20, 25};

// Misc buttons
const SDL_Rect back_btn = {200, 500, 400, 50};

// MessageBox functions
void warn(const char *msg) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "", msg, NULL);
}

bool confirm_action(const char *msg) {
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Cancel" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes"},
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_WARNING,
        NULL,
        "",
        msg,
        sizeof(buttons) / sizeof(SDL_MessageBoxButtonData),
        buttons,
        NULL
    };

    int button_id;
    SDL_ShowMessageBox(&messageboxdata, &button_id);
    return (button_id > 0);
}

bool game_inited = false;

bool menu_button_pressed(SDL_MouseButtonEvent mouse, SDL_Rect button) {
    return (
        mouse.x >= button.x && mouse.x <= button.x + button.w
        && mouse.y >= button.y && mouse.y <= button.y + button.h
    );
}

void menu_handle_mouse_click_customize(SDL_MouseButtonEvent mouse) {
    const char *const disabled = "This setting is disabled while a game is running.";
    const int max_color_scheme_id = sizeof(color_schemes) / sizeof(color_scheme_t) - 1;

    if (game_started())
        if(menu_button_pressed(mouse, size_l_btn)
        || menu_button_pressed(mouse, size_m_btn)) { warn(disabled); return; }

    if (menu_button_pressed(mouse, size_l_btn)) {
        if (board_size > 4) {
            if (game_inited) { game_inited = false; game_deinit(); }
            board_size--;
        }
        return;
    }

    if (menu_button_pressed(mouse, size_m_btn)) {
        if (board_size < 8) {
            if (game_inited) { game_inited = false; game_deinit(); }
            board_size++;
        }
        return;
    }

    if (menu_button_pressed(mouse, sound_btn)) {
        sound ^= true;
        return;
    }

    if (menu_button_pressed(mouse, theme_l_btn)) {
        color_scheme_id--;
        if (color_scheme_id < 0)
            color_scheme_id = max_color_scheme_id;
        color_scheme = color_schemes[color_scheme_id];
        return;
    }

    if (menu_button_pressed(mouse, theme_r_btn)) {
        color_scheme_id++;
        if (color_scheme_id > max_color_scheme_id)
            color_scheme_id = 0;
        color_scheme = color_schemes[color_scheme_id];
        return;
    }
}

void menu_handle_mouse_click(SDL_MouseButtonEvent mouse) {
    if (menu_state != DEFAULT && menu_button_pressed(mouse, back_btn)) {
        menu_state = DEFAULT;
        return;
    }

    if (menu_state == CUSTOMIZE) {
        menu_handle_mouse_click_customize(mouse);
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

    if(game_started() && menu_button_pressed(mouse, reset_btn)) {
        if (!game_over()) {
            bool confirm = confirm_action(
                "The current game session will be lost.\n\n"
                "Are you sure you want to continue?"
            );

            if (!confirm) return;

            game_inited = false;
            game_deinit();
            return;
        }
        game_deinit();
        game_init();
        menu_visible(false);
        return;
    }
}

void render_menu() {
    const SDL_Color btn_color = {255, 255, 255};

    const char *play = !game_over() ? !game_started() ? "Play" : "Resume" : "Review";
    render_button(play_btn, play, color_scheme.buttons_background, btn_color);

    render_button(customize_btn, "Customize", color_scheme.buttons_background, btn_color);
    render_button(rules_btn, "Rules", color_scheme.buttons_background, btn_color);

    if (game_started()) {
        const char* msg = game_over() ? "New Game" : "Cancel Game";
        render_button(reset_btn, msg, color_scheme.buttons_background, btn_color);
    }
}

void render_customize() {
    const SDL_Color btn_color = {255, 255, 255};

    render_text("Board size:", 18, {305, 150}, {0, 0, 0});
    render_rect({445, 152, 30, 25}, {255, 255, 255});
    render_button(size_l_btn, "-", color_scheme.buttons_background, btn_color);
    render_button(size_m_btn, "+", color_scheme.buttons_background, btn_color);

    char size[] = "*";
    size[0] = board_size + '0';
    render_text(size, 15, {455, 152}, {0, 0, 0});

    render_text("Sound:", 18, {340, 200}, {0, 0, 0});
    render_button(sound_btn, sound ? "X" : "", color_scheme.buttons_background, btn_color);

    render_text("Theme:", 18, {340, 250}, {0, 0, 0});
    render_button(theme_l_btn, "<", color_scheme.buttons_background, btn_color);
    render_button(theme_r_btn, ">", color_scheme.buttons_background, btn_color);

    static board_t *board = NULL;
    if (!board) {
        board = board_create(2);
        board_place_piece(board, {0, 0});
        board_place_piece(board, {1, 1});
    }

    render_board(board, {360, 330}, color_scheme);
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
    render_clear(color_scheme.background);
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
        render_button(back_btn, "Back", color_scheme.buttons_background, {255, 255, 255});

    if (window_event.type == SDL_MOUSEBUTTONDOWN)
        menu_handle_mouse_click(window_event.button);
    
    render_present();
}
