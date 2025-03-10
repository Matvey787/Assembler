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
errors readFile(unsigned char** buffer, const char* file_name, size_t* numOfSymbols = nullptr,
                                                               size_t* numOfStrs = nullptr);
errors writeFile   (const unsigned char* buffer, const char* file_name, size_t numOfSymbols);
errors parseString(buffer* confBuff, buffer* comBuff, size_t* offset, bool* codeSectionFound);
errors readFileMmap(unsigned char** buffer, const char* file_name, size_t* numOfSymbols = nullptr, 
                                                                   size_t* numOfStrs = nullptr);

#endif // CRACKALKA_H