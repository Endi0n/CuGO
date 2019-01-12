#include <SDL2/SDL.h>

#include "render.h"
#include "game.h"
#include "menu.h"
#include "sound.h"

int main(int argv, char **args) {
    render_init();
    sound_init();

    SDL_Event window_event;

    for (;;) {
        SDL_WaitEvent(&window_event);

        if (window_event.type == SDL_QUIT) break;

        if (menu_visible()) menu_loop(window_event);
        else game_loop(window_event);
    }

    sound_deinit();
    render_deinit();
    
    return 0;
}
