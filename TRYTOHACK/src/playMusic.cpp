#include "CRACKalka.h"
#include <assert.h>

#include "errors.h"

errors playMusic(const char* mp3FileName) // FIXME sfml
{
    errors err = NO_ERR;
    // Iniтialisation of SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Iniтialisation error of SDL: %s\n", SDL_GetError());
        return SYSTEM_ERR;
    }

    // Initialisation of SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Iniтialisation error of SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return SYSTEM_ERR;
    }

    // load mp3 file
    Mix_Music* music = Mix_LoadMUS(mp3FileName);
    if (!music) {
        printf("Error loading music file: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return FILE_MP3_NOT_FOUND;
    }

    // playback music 
    if (Mix_PlayMusic(music, 1) == -1) 
    {//                      ^------ play music one circle
        printf("Playback error: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return MUS_PLAYBACK_ERR;
    }

    err = drawWindow(music);

    // clear memory, close sdl
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return err;
}
