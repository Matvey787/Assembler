#include "../inc/constants.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

int main()
{
    sf::RenderWindow window(sf::VideoMode(C_SCREEN_WIDTH, C_SCREEN_HEIGHT), "Buterbrod_opt2");
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
                    case sf::Keyboard::Up:
                        offsetY += 0.1;
                        break;
                    case sf::Keyboard::Down:
                        offsetY -= 0.1;
                        break;
                    case sf::Keyboard::Right:
                        offsetX += 0.1;
                        break;
                    case sf::Keyboard::Left:
                        offsetX -= 0.1;
                        break;
                    case sf::Keyboard::Equal:
                        if (event.key.control) {
                            zoomX *= 0.9;
                            zoomY *= 0.9;
                        }
                        break;
                    case sf::Keyboard::Hyphen:
                        if (event.key.control) {
                            zoomX *= 1.1;
                            zoomY *= 1.1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);

        for (size_t y = 0; y < C_SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < C_SCREEN_WIDTH; x += 4) 
        {
                double k_x = zoomX / (double)C_SCREEN_WIDTH;
                double k_y = zoomY / (double)C_SCREEN_HEIGHT;

                double x0_single = (double)x * k_x + offsetX;
                double y0_single = (double)y * k_y  + offsetY;
                
                const int max_iteration = 100;

                double X0[4] = {x0_single, 
                                x0_single + 1 * k_x,
                                x0_single + 2 * k_x,
                                x0_single + 3 * k_x};
                                
                double Y0[4] = {y0_single, y0_single, y0_single, y0_single};

                double X[4] = {}; for (int i = 0; i < 4; i++) X[i] = X0[i];
                double Y[4] = {}; for (int i = 0; i < 4; i++) Y[i] = Y0[i];

                int N[4] = {0, 0, 0, 0};
                for (int n = 0; n < max_iteration; n++) 
                {
                    int mask = 0;
                    for (int i = 0; i < 4; i++) {
                        double x2 = X[i] * X[i];
                        double y2 = Y[i] * Y[i];
                        double r2 = x2 + y2;
                        if (r2 <= 4) {
                            N[i]++;
                            double xy = X[i] * Y[i];
                            X[i] = x2 - y2 + X0[i];
                            Y[i] = xy + xy + Y0[i];
                            mask |= (1 << i);
                        }
                    }
                    if (!mask) break;

                    for (int i = 0; i < 4; i++)
                    {
                        int iteration = N[i];
                        if (iteration == max_iteration)
                        {
                            buffer.setPixel(x + i, y, sf::Color::Black);
                        } else 
                        {
                            float t = (float)iteration / max_iteration;
                            unsigned char r = (unsigned char)(255 * (1.0f) * t * 4);
                            unsigned char g = (unsigned char)(255 *  sqrt(t*M_PI));
                            unsigned char b = (unsigned char)(100 * (1.0f - t * t));

                            buffer.setPixel(x + i, y, sf::Color(r, g, b));
                        }
                    }
                }
            }
        }

        texture.loadFromImage(buffer);
        bufferSprite.setTexture(texture);

        frameCount++;
        float currentTime = clock.getElapsedTime().asSeconds();
        if (currentTime - lastTime >= 1.f) {
            float fps = frameCount / (currentTime - lastTime);
            printf("FPS: %.0f\n", fps);
            frameCount = 0;
            lastTime = currentTime;
            lastTime = currentTime;
        }

        window.clear();
        window.draw(bufferSprite);
        window.display();
    }

    return 0;
}