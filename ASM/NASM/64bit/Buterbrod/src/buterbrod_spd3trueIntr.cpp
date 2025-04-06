#include "../inc/constants.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <immintrin.h>

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

struct screenData
{
    double offsetX;
    double offsetY;
    double zoomX;
    double zoomY;
};

inline void mm_add_epi32(unsigned int mm[4], const unsigned int mm2[4]) 
    { for (int i = 0; i < 4; i++) mm[i] += mm2[i]; }

void getEventFromUser(screenData* screen, sf::RenderWindow* window, bool graphicsEnabled);

void initCoordinates(double X0[4], __m256d* X_vec, __m256d* Y_vec, double x0_single, double y0_single, double k_x);

void iterateMandelbrot(__m256d* X_vec, __m256d* Y_vec, const __m256d X0_vec, const __m256d Y0_vec,
                       unsigned int N[4], unsigned int max_iteration, double c_maxRadius);

void setPixels(sf::Image* buffer, size_t x, size_t y, const unsigned int N[4], int max_iteration);

void countPixels(screenData* screen, sf::Image* buffer);

int main(int argc, char* argv[])
{
    PROFILE_SET_LIMIT_(1000);
    bool graphicsEnabled = false;
    if (strcmp(argv[argc-1], "--onGr") == 0) 
    {
        graphicsEnabled = true;
    }
    else if (strcmp(argv[argc-1], "--offGr") == 0)
    {
        graphicsEnabled = false;
    }

    sf::RenderWindow window(sf::VideoMode(C_SCREEN_WIDTH, C_SCREEN_HEIGHT), "Buterbrod");
    sf::Image buffer;
    buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);
    sf::Texture texture;
    sf::Sprite bufferSprite;

    //unsigned long long lastTicks = __rdtsc();
    screenData screen;
    screen.offsetX = -2.5;
    screen.offsetY = 1;
    screen.zoomX = 3.5;
    screen.zoomY = -2.0;

    size_t counter = 0;
    while (window.isOpen())
    {
        getEventFromUser(&screen, &window, graphicsEnabled);

        if (graphicsEnabled)
        {
            buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);
        }

        countPixels(&screen, &buffer);

        if (graphicsEnabled)
        {
            texture.loadFromImage(buffer);
            bufferSprite.setTexture(texture);
        }

        window.clear();
        if (graphicsEnabled)
        {
            window.draw(bufferSprite);
            window.display();
        }
        
        counter += 1;
    }
    PROFILE_INFO_
    return 0;
}

void countPixels(screenData* screen, sf::Image* buffer)
{
    double offsetX = screen->offsetX;
    double offsetY = screen->offsetY;
    double zoomX = screen->zoomX;
    double zoomY = screen->zoomY;

    PROFILE_START_("cntPxls")
    for (size_t y = 0; y < C_SCREEN_HEIGHT; y++)
    {
        for (size_t x = 0; x < C_SCREEN_WIDTH; x += 4)
        {
            const double k_x = zoomX / (double)C_SCREEN_WIDTH;
            const double k_y = zoomY / (double)C_SCREEN_HEIGHT;

            double x0_single = (double)x * k_x + offsetX;
            double y0_single = (double)y * k_y + offsetY;

            double X0[4];
            __m256d X_vec, Y_vec;
            initCoordinates(X0, &X_vec, &Y_vec, x0_single, y0_single, k_x);

            unsigned int N[4] = {0, 0, 0, 0};
            iterateMandelbrot(&X_vec, &Y_vec, _mm256_loadu_pd(X0), Y_vec, N, max_iteration, c_maxRadius);

            setPixels(buffer, x, y, N, max_iteration);
        }
    }
    PROFILE_END_("cntPxls")
}

void initCoordinates(double X0[4], __m256d* X_vec, __m256d* Y_vec, double x0_single, double y0_single, double k_x)
{
    X0[0] = x0_single;
    X0[1] = x0_single + 1 * k_x;
    X0[2] = x0_single + 2 * k_x;
    X0[3] = x0_single + 3 * k_x;

    *X_vec = _mm256_loadu_pd(X0);
    *Y_vec = _mm256_set1_pd(y0_single);
}

void iterateMandelbrot(__m256d* X_vec, __m256d* Y_vec, const __m256d X0_vec, const __m256d Y0_vec,
                       unsigned int N[4], unsigned int max_iteration, double c_maxRadius)
{
    for (unsigned int n = 0; n < max_iteration; n++) 
    {
        __m256d X2_vec = _mm256_mul_pd(*X_vec, *X_vec);
        __m256d Y2_vec = _mm256_mul_pd(*Y_vec, *Y_vec);
        __m256d XY_vec = _mm256_mul_pd(*X_vec, *Y_vec);
        __m256d R2_vec = _mm256_add_pd(X2_vec, Y2_vec);

        unsigned int cmp[4] = {};
        __m256d maxRadius = _mm256_set1_pd(c_maxRadius);
        __m256d CMP_vec = _mm256_cmp_pd(R2_vec, maxRadius, _CMP_LE_OQ);
        int mask = _mm256_movemask_pd(CMP_vec);
        cmp[0] = (mask & 1);
        cmp[1] = (mask & 2) >> 1;
        cmp[2] = (mask & 4) >> 2;
        cmp[3] = (mask & 8) >> 3;

        if (!mask) break;

        __m128i N_vec = _mm_loadu_si128((__m128i*)N);
        __m128i cmp_vec = _mm_loadu_si128((__m128i*)cmp);
        __m128i N_result = _mm_add_epi32(N_vec, cmp_vec);
        _mm_storeu_si128((__m128i*)N, N_result);

        X2_vec = _mm256_sub_pd(X2_vec, Y2_vec);
        *X_vec = _mm256_add_pd(X2_vec, X0_vec);

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

void getEventFromUser(screenData* screen, sf::RenderWindow* window, bool graphicsEnabled)
{
    double* offsetX = &(screen->offsetX);
    double* offsetY = &(screen->offsetY);
    double* zoomX = &(screen->zoomX);
    double* zoomY = &(screen->zoomY);

    sf::Event event;
    while ((*window).pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
            (*window).close();

        if (graphicsEnabled && event.type == sf::Event::KeyPressed) 
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