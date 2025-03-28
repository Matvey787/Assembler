#include "../inc/constants.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include <immintrin.h>

inline void mm_add_epi32(unsigned int mm[4], const unsigned int mm2[4]) 
    { for (int i = 0; i < 4; i++) mm[i] += mm2[i]; }

void getEventFromUser(double* zoomX, double* zoomY, double* offsetX, double* offsetY,
                      sf::RenderWindow* window);

void initCoordinates(double X0[4], __m256d* X_vec, __m256d* Y_vec, double x0_single, double y0_single, double k_x);

void iterateMandelbrot(__m256d* X_vec, __m256d* Y_vec, const __m256d X0_vec, const __m256d Y0_vec,
                       unsigned int N[4], unsigned int max_iteration, double c_maxRadius);

void setPixels(sf::Image* buffer, size_t x, size_t y, const unsigned int N[4], int max_iteration);

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
        getEventFromUser(&zoomX, &zoomY, &offsetX, &offsetY, &window);

        buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);

        for (size_t y = 0; y < C_SCREEN_HEIGHT; y++) {
            for (size_t x = 0; x < C_SCREEN_WIDTH; x += 4) {
                const double k_x = zoomX / (double)C_SCREEN_WIDTH;
                const double k_y = zoomY / (double)C_SCREEN_HEIGHT;

                double x0_single = (double)x * k_x + offsetX;
                double y0_single = (double)y * k_y + offsetY;

                double X0[4];
                __m256d X_vec, Y_vec;
                initCoordinates(X0, &X_vec, &Y_vec, x0_single, y0_single, k_x);

                unsigned int N[4] = {0, 0, 0, 0};
                iterateMandelbrot(&X_vec, &Y_vec, _mm256_loadu_pd(X0), Y_vec, N, max_iteration, c_maxRadius);

                setPixels(&buffer, x, y, N, max_iteration);
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

void initCoordinates(double X0[4], __m256d* X_vec, __m256d* Y_vec, double x0_single, double y0_single, double k_x)
{
    X0[0] = x0_single;
    X0[1] = x0_single + 1 * k_x;
    X0[2] = x0_single + 2 * k_x;
    X0[3] = x0_single + 3 * k_x;

    // X_vec = [X0[0], X0[1], X0[2], X0[3]]
    *X_vec = _mm256_loadu_pd(X0);
    // Y_vec = [y0_single, y0_single, y0_single, y0_single]
    *Y_vec = _mm256_set1_pd(y0_single);
}

void iterateMandelbrot(__m256d* X_vec, __m256d* Y_vec, const __m256d X0_vec, const __m256d Y0_vec,
                       unsigned int N[4], unsigned int max_iteration, double c_maxRadius)
{
    for (unsigned int n = 0; n < max_iteration; n++) 
    {
        // count x2[4]
        __m256d X2_vec = _mm256_mul_pd(*X_vec, *X_vec);

        // count y2[4]
        __m256d Y2_vec = _mm256_mul_pd(*Y_vec, *Y_vec);
        
        // count xy[4]
        __m256d XY_vec = _mm256_mul_pd(*X_vec, *Y_vec);

        // count r2[4]
        __m256d R2_vec = _mm256_add_pd(X2_vec, Y2_vec);

        // r2[i]
        unsigned int cmp[4] = {};
        __m256d maxRadius = _mm256_set1_pd(c_maxRadius);
        __m256d CMP_vec = _mm256_cmp_pd(R2_vec, maxRadius, _CMP_LE_OQ);
        int mask = _mm256_movemask_pd(CMP_vec);
        cmp[0] = (mask & 1);
        cmp[1] = (mask & 2) >> 1;
        cmp[2] = (mask & 4) >> 2;
        cmp[3] = (mask & 8) >> 3;

        if (!mask) break;

        // N[i] += cmp[i];
        __m128i N_vec = _mm_loadu_si128((__m128i*)N);
        __m128i cmp_vec = _mm_loadu_si128((__m128i*)cmp);
        __m128i N_result = _mm_add_epi32(N_vec, cmp_vec);
        _mm_storeu_si128((__m128i*)N, N_result);

        // X[i] = x2[i] - y2[i] + X0[i]
        X2_vec = _mm256_sub_pd(X2_vec, Y2_vec);
        *X_vec = _mm256_add_pd(X2_vec, X0_vec);

        // Y[i] = xy[i] + xy[i] + y0_single
        __m256d temp_vec = _mm256_add_pd(XY_vec, XY_vec);
        *Y_vec = _mm256_add_pd(temp_vec, Y0_vec);
    }
}

void setPixels(sf::Image* buffer, size_t x, size_t y, const unsigned int N[4], int max_iteration)
{
    for (int i = 0; i < 4; i++)
    {
        int iteration = N[i];
        if (iteration == max_iteration)
        {
            buffer->setPixel(x + i, y, sf::Color::Black);
        } else 
        {
            float t = (float)iteration / max_iteration;
            unsigned char r = (unsigned char)(255 * (1.0f) * t * 4);
            unsigned char g = (unsigned char)(255 * sqrt(t * M_PI));
            unsigned char b = (unsigned char)(100 * (1.0f - t * t));
            buffer->setPixel(x + i, y, sf::Color(r, g, b));
        }
    }
}

void getEventFromUser(double* zoomX, double* zoomY, double* offsetX, double* offsetY,
                      sf::RenderWindow* window)
{
    sf::Event event;
    while ((*window).pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
            (*window).close();
        if (event.type == sf::Event::KeyPressed) 
        {
            switch (event.key.code) {
                case sf::Keyboard::Up:
                    *offsetY += 0.1;
                    break;
                case sf::Keyboard::Down:
                    *offsetY -= 0.1;
                    break;
                case sf::Keyboard::Right:
                    *offsetX += 0.1;
                    break;
                case sf::Keyboard::Left:
                    *offsetX -= 0.1;
                    break;
                case sf::Keyboard::Equal:
                    if (event.key.control) {
                        *zoomX *= 0.9;
                        *zoomY *= 0.9;
                    }
                    break;
                case sf::Keyboard::Hyphen:
                    if (event.key.control) {
                        *zoomX *= 1.1;
                        *zoomY *= 1.1;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}