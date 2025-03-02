#ifndef CRACKALKA_H
#define CRACKALKA_H

#include "errors.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

errors playMusic(const char* mp3FileName);
errors drawWindow(Mix_Music* music);
errors readFile(FILE* file, unsigned char* buffer, size_t* numOfChars);
errors procComFile(const char* comFileName);
void crackFile(unsigned char* buffer, size_t numOfChars);



#endif // CRACKALKA_H