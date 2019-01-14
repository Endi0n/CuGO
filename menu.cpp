#include <SDL2/SDL.h>
#include <fstream>

#include "menu.h"
#include "dimensions.h"
#include "game.h"
#include "render.h"

// Menu visibility getter and setter
bool menu_show = true;
bool menu_visible() { return menu_show; }
bool menu_visible(bool visible) { menu_show = visible; return visible; }

// Color scheme getter and setter
int color_scheme_id = 0;
color_scheme_t menu_color_scheme() { return color_schemes[color_scheme_id]; }

// Sound active getter and setter
bool sound = true;
bool menu_sound() { return sound; }

// Board size getter and setter
int board_size = 8;
uint_t menu_board_size() { return board_size; }

// Suicide check getter and setter
bool prevent_suicide = true;
bool menu_prevent_suicide() { return prevent_suicide; }

// Menu state 
enum menu_state_e {
    DEFAULT,
    CUSTOMIZE,
    RULES,
};

menu_state_e menu_state = DEFAULT;

void menu_init() {
    std::ifstream fin("settings");
    if (!fin.good()) return;
    fin >> color_scheme_id >> sound >> board_size >> prevent_suicide;
    fin.close();
}

void menu_deinit() {
    std::ofstream fout("settings");
    fout << color_scheme_id << ' ' << sound << ' ' << board_size << ' ' << prevent_suicide;
    fout.close();
}

// Customization menu buttons
const SDL_Rect sound_btn = {446, 151, 30, 25};
const SDL_Rect size_l_btn = {415, 202, 20, 25};
const SDL_Rect size_m_btn = {485, 202, 20, 25};
const SDL_Rect suicide_btn = {500, 251, 30, 25};
const SDL_Rect theme_l_btn = {438, 302, 20, 25};
const SDL_Rect theme_r_btn = {463, 302, 20, 25};

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
        SDL_MESSAGEBOX_WARNING, NULL, "", msg,
        sizeof(buttons) / sizeof(SDL_MessageBoxButtonData), buttons,
        NULL
    };

    int button_id;
    SDL_ShowMessageBox(&messageboxdata, &button_id);

    return (button_id > 0);
}

bool game_inited = false;

bool menu_button_pressed(SDL_MouseButtonEvent mouse, SDL_Rect button) {
    return (
        mouse.x >= button.x && mouse.x <= button.x + button.w &&
        mouse.y >= button.y && mouse.y <= button.y + button.h
    );
}

SDL_Rect menu_button_position(uint_t index) {
    return {200, 150 + 80 * (int)index, 400, 50};
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

    if (menu_button_pressed(mouse, suicide_btn)) {
        prevent_suicide ^= true;
        return;
    }

    if (menu_button_pressed(mouse, theme_l_btn)) {
        color_scheme_id--;
        if (color_scheme_id < 0)
            color_scheme_id = max_color_scheme_id;
        return;
    }

    if (menu_button_pressed(mouse, theme_r_btn)) {
        color_scheme_id++;
        if (color_scheme_id > max_color_scheme_id)
            color_scheme_id = 0;
        return;
    }
}

void menu_handle_mouse_click(SDL_MouseButtonEvent mouse) {
    if (menu_state != DEFAULT && menu_button_pressed(mouse, back_btn))
        menu_state = DEFAULT;

    if (menu_state == CUSTOMIZE) {
        menu_handle_mouse_click_customize(mouse);
        return;
    }
    
    if(menu_button_pressed(mouse, menu_button_position(0))) {
        if (!game_inited) {
            game_inited = true;
            game_init();
        }
        menu_visible(false);
        return;
    }

    if (menu_button_pressed(mouse, menu_button_position(1))) {
        menu_state = CUSTOMIZE;
        return;
    }

    if(menu_button_pressed(mouse, menu_button_position(2))) {
        menu_state = RULES;
        return;
    }

    if(game_started() && menu_button_pressed(mouse, menu_button_position(3))) {
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
    render_button(menu_button_position(0), play, menu_color_scheme().buttons_background, btn_color);

    render_button(menu_button_position(1), "Customize", menu_color_scheme().buttons_background, btn_color);
    render_button(menu_button_position(2), "Rules", menu_color_scheme().buttons_background, btn_color);

    if (game_started()) {
        const char* msg = game_over() ? "New Game" : "Cancel Game";
        render_button(menu_button_position(3), msg, menu_color_scheme().buttons_background, btn_color);
    }
}

void render_customize() {
    const SDL_Color btn_color = {255, 255, 255};

    render_text("Sound:", 18, {340, 150}, {0, 0, 0});
    render_button(sound_btn, sound ? "X" : "", menu_color_scheme().buttons_background, btn_color);

    render_text("Board size:", 18, {305, 200}, {0, 0, 0});
    render_rect({445, 202, 30, 25}, {255, 255, 255});
    render_button(size_l_btn, "-", menu_color_scheme().buttons_background, btn_color);
    render_button(size_m_btn, "+", menu_color_scheme().buttons_background, btn_color);
    char size[] = "*";
    size[0] = board_size + '0';
    render_text(size, 15, {455, 202}, {0, 0, 0});

    render_text("Prevent suicidal moves:", 18, {285, 250}, {0, 0, 0});
    render_button(suicide_btn, prevent_suicide ? "X" : "", menu_color_scheme().buttons_background, btn_color);

    render_text("Theme:", 18, {340, 300}, {0, 0, 0});
    render_button(theme_l_btn, "<", menu_color_scheme().buttons_background, btn_color);
    render_button(theme_r_btn, ">", menu_color_scheme().buttons_background, btn_color);

    static board_t *board = NULL;
    if (!board) {
        board = board_create(2);
        board_place_piece(board, {0, 0}, false);
        board_place_piece(board, {1, 1}, false);
    }

    render_board(board, {360, 365}, menu_color_scheme());
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
        render_circle(3, {offset_x + 15, 167 + 18 * i}, {0, 0, 0});
        render_text(rules[i], 14, {offset_x + 25, 157 + 18 * i}, {0, 0, 0});
    }
}

void menu_loop(SDL_Event window_event) {
    render_clear(menu_color_scheme().background);
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
        render_button(back_btn, "Back", menu_color_scheme().buttons_background, {255, 255, 255});

    if (window_event.type == SDL_MOUSEBUTTONDOWN)
        menu_handle_mouse_click(window_event.button);
    
    render_present();
}
