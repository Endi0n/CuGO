#include <SDL2/SDL_mixer.h>

Mix_Chunk *sound_place_piece;

void sound_init() {
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512); 
    sound_place_piece = Mix_LoadWAV("place.wav");
}

void sound_deinit() {
    Mix_FreeChunk(sound_place_piece);
    Mix_CloseAudio();
}

void sound_play_place_piece() {
    Mix_PlayChannel(-1, sound_place_piece, 0);
}
