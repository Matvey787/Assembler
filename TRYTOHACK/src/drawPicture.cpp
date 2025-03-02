#include "constants.h"
#include "CRACKalka.h"
#include "errors.h"

#include <SFML/Graphics.hpp> // need for drawing window

errors drawWindow(Mix_Music* music)
{
    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode(IMG_WIDTH, IMG_HEIGHT), "CRACKalka");

    // Load the texture
    sf::Texture texture;
    if (!texture.loadFromFile("duck.png")) {
        fprintf(stderr, "Error: Could not load duck.png");
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