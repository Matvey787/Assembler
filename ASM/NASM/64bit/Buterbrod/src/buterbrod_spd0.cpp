#include "../inc/constants.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

int main()
{
    sf::RenderWindow window(sf::VideoMode(C_SCREEN_WIDTH, C_SCREEN_HEIGHT), "Mandelbrot");
    sf::Image buffer;
    buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);
    sf::Texture texture;
    sf::Sprite bufferSprite;

    sf::Clock clock; // for FPS
    float lastTime = 0.f;
    int frameCount = 0;

    double offsetX = -2.5;
    double offsetY = 1;
    double zoomX = 3.5;
    double zoomY = 2.0;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
                
            if (event.type == sf::Event::KeyPressed) 
            {
                switch (event.key.code) 
                {
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

        // Buterbrod
        for (size_t x = 0; x < C_SCREEN_WIDTH; x++) {
            for (size_t y = 0; y < C_SCREEN_HEIGHT; y++) 
            {
                double x0 = (x / (double)C_SCREEN_WIDTH) * zoomX + offsetX;
                double y0 = (y / (double)C_SCREEN_HEIGHT) * zoomY - offsetY;
                
                double x_new = 0, y_new = 0;
                int iteration = 0;
                const int max_iteration = 100;

                while (x_new * x_new + y_new * y_new <= 4 && iteration < max_iteration) 
                {

                    double xtemp = x_new * x_new - y_new * y_new + x0;
                    y_new = 2 * x_new * y_new + y0;
                    x_new = xtemp;
                    iteration++;
                }

                // sf::Color color(iteration, iteration, iteration);
                // buffer.setPixel(x, y, color);

                if (iteration == max_iteration)
                {
                    buffer.setPixel(x, y, sf::Color::Black);
                } else 
                {
                    float t = (float)iteration / max_iteration;
                    unsigned char r = (unsigned char)(255 * (1.0f) * t * 4);
                    unsigned char g = (unsigned char)(255 *  sqrt(t*M_PI));
                    unsigned char b = (unsigned char)(100 * (1.0f - t * t));

                    buffer.setPixel(x, y, sf::Color(r, g, b));
                }
            }
        }

        texture.loadFromImage(buffer);
        bufferSprite.setTexture(texture);

        // FPS
        frameCount++;
        float currentTime = clock.getElapsedTime().asSeconds();
        if (currentTime - lastTime >= 1.f) {
            float fps = frameCount / (currentTime - lastTime);
            printf("FPS: %.0f\n", fps);
            frameCount = 0;
            lastTime = currentTime;
        }

        window.clear();
        window.draw(bufferSprite);
        window.display();
    }

    return 0;
}
