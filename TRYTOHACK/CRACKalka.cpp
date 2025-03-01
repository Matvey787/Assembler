#include <SFML/Graphics.hpp> // need for drawing window
#include <SDL2/SDL.h>        // need for iniтialisation of music
#include <SDL2/SDL_mixer.h>  // need for iniтialisation of music
#include <iostream>
#include <assert.h>
#include "errors.h"

errors playMusic(const char* mp3FileName);
errors drawWindow(Mix_Music* music);
errors readFile(FILE* file, unsigned char* buffer, size_t* numOfChars);
errors procComFile(const char* comFileName);
void crackFile(unsigned char* buffer, size_t numOfChars);

const size_t KByte = 1024; 
const size_t IMG_WIDTH = 300;
const size_t IMG_HEIGHT = 200;
const size_t JNE_ASCII_CODE = 117;

// We go through the *.com file and replace byte after jne with 04 - address of success.
int main(int argc, char const *argv[])
{
    errors err = NO_ERR;
    err = procComFile("/home/matvey/DOSBox/Temp/FIRST_~1.COM");
    if (err != NO_ERR)
    {
        printf("Something go wrong during processing com file!\n");
        return err;
    }
    err = playMusic("sigma.mp3"); // play music and call func which draws window  
    return err;
}

errors procComFile(const char* comFileName)
{
    assert(comFileName != nullptr);

    errors err = NO_ERR;
    FILE* file = fopen(comFileName, "r+");

    if (file == nullptr) 
    {
        return FILE_COM_NOT_FOUND;
    }

    unsigned char buffer[KByte] = {0};
    size_t numOfChars = 0;

    err = readFile(file, buffer, &numOfChars);
    crackFile(buffer, numOfChars);
    

    // rewrite file
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < numOfChars; i++)
    {
        putc(buffer[i], file);
    }

    if (err != NO_ERR)
        printf("File successfully cracked!\n");
    
    fclose(file);
    return err;
}

void crackFile(unsigned char* buffer, size_t numOfChars)
{
    assert(buffer != nullptr);

    int stopCounter = 0;
    for (int i = 0; i < numOfChars; i++)
    {
        //printf("%x\n", buffer[i]);
        if (isalpha(buffer[i]))
        {
            ++stopCounter;
        }
        else
        {
            stopCounter = 0;
        }
        if (stopCounter == 3)
            break;

        if (buffer[i] == JNE_ASCII_CODE)
        {//              ^------ ascii code of jne (75 in hex)
            //printf("---------------- %x\n", buffer[i]);
            buffer[i + 1] = 4; // 4 - address of success
        }
    }
}

errors readFile(FILE* file, unsigned char* buffer, size_t* numOfChars)
{
    assert(file != nullptr);
    assert(buffer != nullptr);

    *numOfChars = fread(buffer, sizeof(*buffer), KByte, file);

    if (numOfChars == 0) 
    {
        printf("File is empty!\n");
        return EMPTY_FILE;
    }

    printf("File content:\n-------------------- start --------------------\n");
    for (size_t i = 0; i < *numOfChars; ++i) 
    {
        if (i % 16 == 0 && i != 0)
        {
            printf("\n");
        }

        if (buffer[i] >= 32 && buffer[i] <= 126)
        {
            printf("%lc ", buffer[i]);
        }
        else
        {
            printf("%x ", buffer[i]);
        }
    }
    printf("\n-------------------- end --------------------\n");
    return NO_ERR;
}

errors playMusic(const char* mp3FileName)
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

errors drawWindow(Mix_Music* music)
{
    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode(IMG_WIDTH, IMG_HEIGHT), "CRACKalka");

    // Load the texture
    sf::Texture texture;
    if (!texture.loadFromFile("duck.png")) {
        std::cerr << "Error: Could not load duck.png" << std::endl;
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return FILE_PNG_NOT_FOUND;
    }

    // Create a sprite
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Close the window when the close button is clicked
            }
        }

        // Check if the music has finished playing
        if (!Mix_PlayingMusic()) {
            window.close(); // Close the window if the music has ended
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return NO_ERR;
}