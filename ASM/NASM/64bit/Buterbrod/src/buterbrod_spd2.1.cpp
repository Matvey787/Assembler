#include "../inc/constants.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

#ifdef ON_STAT
    #include "profiler.h"
    #define PROFILE_START_(name) profileStart(name);
    #define PROFILE_END_(name) profileEnd(name);
    #define PROFILE_SET_LIMIT_(limit) setProfileLimit(limit);
    #define PROFILE_INFO_ printStats();
#else
    #define PROFILE_START_(name)
    #define PROFILE_END_(name)
    #define PROFILE_SET_LIMIT_(limit)
    #define PROFILE_INFO_
#endif

void countPixels(sf::Image& buffer, size_t width, size_t height, double offsetX, double offsetY, 
    double zoomX, double zoomY);

int main()
{
    PROFILE_SET_LIMIT_(1000)
    sf::RenderWindow window(sf::VideoMode(C_SCREEN_WIDTH, C_SCREEN_HEIGHT), "Buterbrod_opt1");
    sf::Image buffer;
    buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);
    sf::Texture texture;
    sf::Sprite bufferSprite;

    sf::Clock clock;
    float lastTime = 0.f;
    int frameCount = 0;

    double offsetX = -2.5;
    double offsetY = 1;
    double zoomX = 3.5;
    double zoomY = -2.0;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up: offsetY += 0.1; break;
                    case sf::Keyboard::Down: offsetY -= 0.1; break;
                    case sf::Keyboard::Right: offsetX += 0.1; break;
                    case sf::Keyboard::Left: offsetX -= 0.1; break;
                    case sf::Keyboard::Equal:
                        if (event.key.control) { zoomX *= 0.9; zoomY *= 0.9; }
                        break;
                    case sf::Keyboard::Hyphen:
                        if (event.key.control) { zoomX *= 1.1; zoomY *= 1.1; }
                        break;
                    default: break;
                }
            }
        }

        buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);
        countPixels(buffer, C_SCREEN_WIDTH, C_SCREEN_HEIGHT, offsetX, offsetY, zoomX, zoomY);
        texture.loadFromImage(buffer);
        bufferSprite.setTexture(texture);

        frameCount++;
        // float currentTime = clock.getElapsedTime().asSeconds();
        // if (currentTime - lastTime >= 1.f) {
        //     float fps = frameCount / (currentTime - lastTime);
        //     printf("FPS: %.0f\n", fps);
        //     frameCount = 0;
        //     lastTime = currentTime;
        // }

        window.clear();
        window.draw(bufferSprite);
        window.display();
    }
    PROFILE_INFO_
    return 0;
}

void countPixels(sf::Image& buffer, size_t width, size_t height, double offsetX, double offsetY, 
                    double zoomX, double zoomY) {
    PROFILE_START_("cntPxls")
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x += 8) 
        {
            const double k_x = zoomX / (double)C_SCREEN_WIDTH;
            const double k_y = zoomY / (double)C_SCREEN_HEIGHT;

            double x0_single = (double)x * k_x + offsetX;
            double y0_single = (double)y * k_y + offsetY;
            
            const int max_iteration = 100;

            double X0[8] = {x0_single,
                            x0_single + 1 * k_x,
                            x0_single + 2 * k_x,
                            x0_single + 3 * k_x,
                            x0_single + 4 * k_x,
                            x0_single + 5 * k_x,
                            x0_single + 6 * k_x,
                            x0_single + 7 * k_x
                        };

            double X[8] = {}; for (int i = 0; i < 8; i++) X[i] = X0[i];
            double Y[8] = {}; for (int i = 0; i < 8; i++) Y[i] = y0_single;

            unsigned int N[8] = {0, 0, 0, 0, 0, 0, 0};
            for (unsigned int n = 0; n < max_iteration; n++) 
            {
                double x2[8] = {}; for (int i = 0; i < 8; i++) x2[i] = X[i] * X[i];
                double y2[8] = {}; for (int i = 0; i < 8; i++) y2[i] = Y[i] * Y[i];
                double xy[8] = {}; for (int i = 0; i < 8; i++) xy[i] = X[i] * Y[i];
                double r2[8] = {}; for (int i = 0; i < 8; i++) r2[i] = x2[i] + y2[i];

                unsigned int cmp[8] = {};
                for (int i = 0; i < 8; i++) cmp[i] = (r2[i] <= 4);

                bool is_out = true;
                for (int i = 0; i < 8; i++) {
                    if (cmp[i]) 
                        is_out = false;
                }
                if (is_out) break;

                for (int i = 0; i < 8; i++) N[i] += cmp[i];

                for (int i = 0; i < 8; i++) X[i] = x2[i] - y2[i] + X0[i];
                for (int i = 0; i < 8; i++) Y[i] = xy[i] + xy[i] + y0_single;
            }
            
            for (int i = 0; i < 8 && (x + i) < C_SCREEN_WIDTH; i++)
            {
                int iteration = N[i];
                if (iteration == max_iteration)
                {
                    buffer.setPixel(x + i, y, sf::Color::Black);
                }
                else 
                {
                    float t = (float)iteration / max_iteration;
                    unsigned char r = (unsigned char)(255 * t * 4);
                    unsigned char g = (unsigned char)(255 * sqrt(t * M_PI));
                    unsigned char b = (unsigned char)(100 * (1.0f - t * t));
                    buffer.setPixel(x + i, y, sf::Color(r, g, b));
                }
            }
        }
    }
    PROFILE_END_("cntPxls")
}