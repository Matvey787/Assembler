#include "../inc/constants.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>



inline void mm_set_ps1(double mm[4], double val)    { for (int i = 0; i < 4; i++) mm[i] = val; }
inline void mm_cpy_ps(double mm[4], double mm2[4])  { for (int i = 0; i < 4; i++) mm[i] = mm2[i]; }

inline void mm_add_ps(double mm[4], const double mm1[4], double mm2[4]) 
    { for (int i = 0; i < 4; i++) mm[i] = mm1[i] + mm2[i]; }
inline void mm_sub_ps(double mm[4], const double mm1[4], double mm2[4]) 
    { for (int i = 0; i < 4; i++) mm[i] = mm1[i] - mm2[i]; }
inline void mm_mul_ps(double mm[4], const double mm1[4], double mm2[4]) 
    { for (int i = 0; i < 4; i++) mm[i] = mm1[i] * mm2[i]; }

inline void mm_add_epi32(unsigned int mm[4], const unsigned int mm2[4]) 
    { for (int i = 0; i < 4; i++) mm[i] += mm2[i]; }

inline void mm_cmple_ps(unsigned int cmp[4], const double mm[4], double val)
    { for (int i = 0; i < 4; i++) cmp[i] = (mm[i] <= val) ? 1 : 0; }


int main()
{
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
            for (size_t x = 0; x < C_SCREEN_WIDTH; x += 4) {
                const double k_x = zoomX / (double)C_SCREEN_WIDTH;
                const double k_y = zoomY / (double)C_SCREEN_HEIGHT;

                double x0_single = (double)x * k_x + offsetX;
                double y0_single = (double)y * k_y  + offsetY;
                
                const int max_iteration = 100;

                double X0[4] = {x0_single, 
                                x0_single + 1 * k_x,
                                x0_single + 2 * k_x,
                                x0_single + 3 * k_x};

                double X[4] = {}; mm_cpy_ps(X, X0);
                // for (int i = 0; i < 4; i++) X[i] = X0[i];
                double Y[4] = {}; mm_set_ps1(Y, y0_single);
                // for (int i = 0; i < 4; i++) Y[i] = y0_single;

                unsigned int N[4] = {0, 0, 0, 0};
                for (unsigned int n = 0; n < max_iteration; n++) 
                {
                    double x2[4] = {}; mm_mul_ps(x2, X, X);
                    // for (int i = 0; i < 4; i++) x2[i] = X[i] * X[i];
                    double y2[4] = {}; mm_mul_ps(y2, Y, Y);
                    // for (int i = 0; i < 4; i++) y2[i] = Y[i] * Y[i];
                    double xy[4] = {}; mm_mul_ps(xy, X, Y);
                    // for (int i = 0; i < 4; i++) xy[i] = X[i] * Y[i];
                    double r2[4] = {}; mm_add_ps(r2, x2, y2);
                    // for (int i = 0; i < 4; i++) r2[i] = x2[i] + y2[i];

                    unsigned int cmp[4] = {};
                    // for (int i = 0; i < 4; i++) cmp[i] = (r2[i] <= 4);

                    mm_cmple_ps(cmp, r2, с_maxRadius);

                    bool is_out = true;
                    for (int i = 0; i < 4; i++) {
                        if (cmp[i]) 
                            is_out = false;
                    }
                    if (is_out) break;
                    
                    mm_add_epi32(N, cmp);
                    // for (int i = 0; i < 4; i++) N[i] += cmp[i];

                    for (int i = 0; i < 4; i++) X[i] = x2[i] - y2[i] + X0[i];
                    for (int i = 0; i < 4; i++) Y[i] = xy[i] + xy[i] + y0_single;
                }
                
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

        texture.loadFromImage(buffer);
        bufferSprite.setTexture(texture);

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