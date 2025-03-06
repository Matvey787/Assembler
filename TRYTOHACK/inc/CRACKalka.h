#ifndef CRACKALKA_H
#define CRACKALKA_H

#include "errors.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

struct buffer
{
    unsigned char* data;
    size_t size;
};

errors playMusic   (const char* mp3FileName);
errors playMusicAndDrawPictureSfml(const char* wavfileName, const char* pngFileName);
errors drawWindow  (Mix_Music* music);
errors procComFile(const char* configFileName, const char* comFileName);
errors crackFile(buffer* comBuff, buffer* confBuff);
void readFile(unsigned char** buffer, const char* file_name, size_t* numOfSymbols,
                                                             size_t* numOfStrs);
errors writeFile   (const unsigned char* buffer, const char* file_name, size_t numOfSymbols);

#endif // CRACKALKA_H