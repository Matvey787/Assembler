#include "../inc/constants.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
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

#ifdef ON_GR
    #define CREATE_BUFFER_                                                                      \
        sf::Image buffer;                                                                       \
        buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);                       \
        sf::Texture texture;                                                                    \
        sf::Sprite bufferSprite;                                             

    #define ON_BUFF_(...) __VA_ARGS__                                                               
#endif

#ifndef ON_GR
    #define CREATE_BUFFER_
    #define ON_BUFF_(...)
#endif

#ifdef NAIVE_V
    #define COUNTPIXELS_ PROFILE_START_("cntPxls")                                  \
    for (size_t x = 0; x < C_SCREEN_WIDTH; x++) {                                   \
        for (size_t y = 0; y < C_SCREEN_HEIGHT; y++)                                \
        {                                                                           \
            double x0 = (x / (double)C_SCREEN_WIDTH) * zoomX + offsetX;             \
            double y0 = (y / (double)C_SCREEN_HEIGHT) * zoomY - offsetY;            \
            double x_new = 0, y_new = 0;                                            \
            int iteration = 0;                                                      \
            const int max_iteration = 100;                                          \
            while (x_new * x_new + y_new * y_new <= 4 && iteration < max_iteration) \
            {                                                                       \
                double xtemp = x_new * x_new - y_new * y_new + x0;                  \
                y_new = 2 * x_new * y_new + y0;                                     \
                x_new = xtemp;                                                      \
                iteration++;                                                        \
            }                                                                       \
            ON_BUFF_                                                                \
            (                                                                       \
                if (iteration == max_iteration)                                     \
                {                                                                   \
                    buffer.setPixel(x, y, sf::Color::Black);                        \
                } else                                                              \
                {                                                                   \
                    float t = (float)iteration / max_iteration;                     \
                    unsigned char r = (unsigned char)(255 * (1.0f) * t * 4);        \
                    unsigned char g = (unsigned char)(255 *  sqrt(t*M_PI));         \
                    unsigned char b = (unsigned char)(100 * (1.0f - t * t));        \
                    buffer.setPixel(x, y, sf::Color(r, g, b));                      \
                }                                                                   \
            )                                                                       \
        }                                                                           \
    }                                                                               \
    PROFILE_END_("cntPxls")   
#endif

#ifdef SPD1_V                                                                     
    #define COUNTPIXELS_ PROFILE_START_("cntPxls")                                \
    for (size_t y = 0; y < C_SCREEN_HEIGHT; y++) {                                \
        for (size_t x = 0; x < C_SCREEN_WIDTH; x += 4) {                          \
            const double k_x = zoomX / (double)C_SCREEN_WIDTH;                    \
            const double k_y = zoomY / (double)C_SCREEN_HEIGHT;                   \
            const double x0_single = (double)x * k_x + offsetX;                   \
            const double y0_single = (double)y * k_y - offsetY;                   \
            const int max_iteration = 100;                                        \
            double X0[4] = {x0_single,                                            \
                            x0_single + k_x,                                      \
                            x0_single + 2 * k_x,                                  \
                            x0_single + 3 * k_x};                                 \
            double X[4] = {X0[0], X0[1], X0[2], X0[3]};                           \
            double Y[4] = {y0_single, y0_single, y0_single, y0_single};           \
            unsigned int N[4] = {0, 0, 0, 0};                                     \
            for (unsigned int n = 0; n < max_iteration; n++)                      \
            {                                                                     \
                bool is_out = true;                                               \
                for (int i = 0; i < 4; i++) {                                     \
                    double x2 = X[i] * X[i];                                      \
                    double y2 = Y[i] * Y[i];                                      \
                    double r2 = x2 + y2;                                          \
                                                                                  \
                    if (r2 <= 4) {                                                \
                        is_out = false;                                           \
                        N[i]++;                                                   \
                        double xy = X[i] * Y[i];                                  \
                        X[i] = x2 - y2 + X0[i];                                   \
                        Y[i] = xy + xy + y0_single;                               \
                    }                                                             \
                }                                                                 \
                if (is_out) break;                                                \
            }                                                                     \
            ON_BUFF_(                                                             \
                for (int i = 0; i < 4; i++)                                       \
                {                                                                 \
                    int iteration = N[i];                                         \
                    if (iteration == max_iteration)                               \
                    {                                                             \
                        buffer.setPixel(x + i, y, sf::Color::Black);              \
                    }                                                             \
                    else                                                          \
                    {                                                             \
                        float t = (float)iteration / max_iteration;               \
                        unsigned char r = (unsigned char)(255 * t * 4);           \
                        unsigned char g = (unsigned char)(255 * sqrt(t * M_PI));  \
                        unsigned char b = (unsigned char)(100 * (1.0f - t * t));  \
                        buffer.setPixel(x + i, y, sf::Color(r, g, b));            \
                    }                                                             \
                }                                                                 \
                )                                                                 \
        }                                                                         \
    }                                                                             \
    PROFILE_END_("cntPxls")                                                       
#endif

#ifdef AVX4
#define COUNTPIXELS_ PROFILE_START_("cntPxls")                                          \
                                                                                        \
    for (size_t y = 0; y < C_SCREEN_HEIGHT; y++)                                        \
    {                                                                                   \
        for (size_t x = 0; x < C_SCREEN_WIDTH; x += 4) {                                \
            const double k_x = zoomX / (double)C_SCREEN_WIDTH;                          \
            const double k_y = zoomY / (double)C_SCREEN_HEIGHT;                         \
                                                                                        \
            double x0_single = (double)x * k_x + offsetX;                               \
            double y0_single = (double)y * k_y  - offsetY;                              \
                                                                                        \
            const int max_iteration = 100;                                              \
                                                                                        \
            double X0[4] = {x0_single,                                                  \
                            x0_single + 1 * k_x,                                        \
                            x0_single + 2 * k_x,                                        \
                            x0_single + 3 * k_x};                                       \
                                                                                        \
            double X[4] = {}; for (int i = 0; i < 4; i++) X[i] = X0[i];                 \
            double Y[4] = {}; for (int i = 0; i < 4; i++) Y[i] = y0_single;             \
                                                                                        \
            unsigned int N[4] = {0, 0, 0, 0};                                           \
            for (unsigned int n = 0; n < max_iteration; n++)                            \
            {                                                                           \
                double x2[4] = {}; for (int i = 0; i < 4; i++) x2[i] = X[i] * X[i];     \
                double y2[4] = {}; for (int i = 0; i < 4; i++) y2[i] = Y[i] * Y[i];     \
                double xy[4] = {}; for (int i = 0; i < 4; i++) xy[i] = X[i] * Y[i];     \
                double r2[4] = {}; for (int i = 0; i < 4; i++) r2[i] = x2[i] + y2[i];   \
                                                                                        \
                unsigned int cmp[4] = {};                                               \
                for (int i = 0; i < 4; i++) cmp[i] = (r2[i] <= 4);                      \
                                                                                        \
                bool is_out = true;                                                     \
                for (int i = 0; i < 4; i++) {                                           \
                    if (cmp[i])                                                         \
                        is_out = false;                                                 \
                }                                                                       \
                if (is_out) break;                                                      \
                                                                                        \
                for (int i = 0; i < 4; i++) N[i] += cmp[i];                             \
                                                                                        \
                for (int i = 0; i < 4; i++) X[i] = x2[i] - y2[i] + X0[i];               \
                for (int i = 0; i < 4; i++) Y[i] = xy[i] + xy[i] + y0_single;           \
            }                                                                           \
                                                                                        \
            ON_BUFF_(                                                                   \
                for (int i = 0; i < 4; i++)                                             \
                {                                                                       \
                    int iteration = N[i];                                               \
                    if (iteration == max_iteration)                                     \
                    {                                                                   \
                        buffer.setPixel(x + i, y, sf::Color::Black);                    \
                    }                                                                   \
                    else                                                                \
                    {                                                                   \
                        float t = (float)iteration / max_iteration;                     \
                        unsigned char r = (unsigned char)(255 * t * 4);                 \
                        unsigned char g = (unsigned char)(255 * sqrt(t * M_PI));        \
                        unsigned char b = (unsigned char)(100 * (1.0f - t * t));        \
                        buffer.setPixel(x + i, y, sf::Color(r, g, b));                  \
                    }                                                                   \
                }                                                                       \
                )                                                                       \
        }                                                                               \
    }                                                                                   \
    PROFILE_END_("cntPxls")                                                       
#endif

#ifdef AVX8                                                                       
#define COUNTPIXELS_ PROFILE_START_("cntPxls")                                          \
    for (size_t y = 0; y < C_SCREEN_HEIGHT; y++)                                        \
    {                                                                                   \
        for (size_t x = 0; x < C_SCREEN_WIDTH; x += 8)                                  \
        {                                                                               \
            const double k_x = zoomX / (double)C_SCREEN_WIDTH;                          \
            const double k_y = zoomY / (double)C_SCREEN_HEIGHT;                         \
                                                                                        \
            double x0_single = (double)x * k_x + offsetX;                               \
            double y0_single = (double)y * k_y - offsetY;                               \
                                                                                        \
            const int max_iteration = 100;                                              \
                                                                                        \
            double X0[8] = {x0_single,                                                  \
                            x0_single + 1 * k_x,                                        \
                            x0_single + 2 * k_x,                                        \
                            x0_single + 3 * k_x,                                        \
                            x0_single + 4 * k_x,                                        \
                            x0_single + 5 * k_x,                                        \
                            x0_single + 6 * k_x,                                        \
                            x0_single + 7 * k_x                                         \
                        };                                                              \
                                                                                        \
            double X[8] = {}; for (int i = 0; i < 8; i++) X[i] = X0[i];                 \
            double Y[8] = {}; for (int i = 0; i < 8; i++) Y[i] = y0_single;             \
                                                                                        \
            unsigned int N[8] = {0, 0, 0, 0, 0, 0, 0};                                  \
            for (unsigned int n = 0; n < max_iteration; n++)                            \
            {                                                                           \
                double x2[8] = {}; for (int i = 0; i < 8; i++) x2[i] = X[i] * X[i];     \
                double y2[8] = {}; for (int i = 0; i < 8; i++) y2[i] = Y[i] * Y[i];     \
                double xy[8] = {}; for (int i = 0; i < 8; i++) xy[i] = X[i] * Y[i];     \
                double r2[8] = {}; for (int i = 0; i < 8; i++) r2[i] = x2[i] + y2[i];   \
                                                                                        \
                unsigned int cmp[8] = {};                                               \
                for (int i = 0; i < 8; i++) cmp[i] = (r2[i] <= 4);                      \
                                                                                        \
                bool is_out = true;                                                     \
                for (int i = 0; i < 8; i++) {                                           \
                    if (cmp[i])                                                         \
                        is_out = false;                                                 \
                }                                                                       \
                if (is_out) break;                                                      \
                                                                                        \
                for (int i = 0; i < 8; i++) N[i] += cmp[i];                             \
                                                                                        \
                for (int i = 0; i < 8; i++) X[i] = x2[i] - y2[i] + X0[i];               \
                for (int i = 0; i < 8; i++) Y[i] = xy[i] + xy[i] + y0_single;           \
            }                                                                           \
                                                                                        \
            ON_BUFF_(                                                                   \
                for (int i = 0; i < 8 && (x + i) < C_SCREEN_WIDTH; i++)                 \
                {                                                                       \
                    int iteration = N[i];                                               \
                    if (iteration == max_iteration)                                     \
                    {                                                                   \
                        buffer.setPixel(x + i, y, sf::Color::Black);                    \
                    }                                                                   \
                    else                                                                \
                    {                                                                   \
                        float t = (float)iteration / max_iteration;                     \
                        unsigned char r = (unsigned char)(255 * t * 4);                 \
                        unsigned char g = (unsigned char)(255 * sqrt(t * M_PI));        \
                        unsigned char b = (unsigned char)(100 * (1.0f - t * t));        \
                        buffer.setPixel(x + i, y, sf::Color(r, g, b));                  \
                    }                                                                   \
                }                                                                       \
            )                                                                           \
        }                                                                               \
    }                                                                                   \
    PROFILE_END_("cntPxls")                                                       
#endif                                                                            

#ifdef TRUEAVX256
#define COUNTPIXELS_ PROFILE_START_("cntPxls")                                    \
    for (size_t y = 0; y < C_SCREEN_HEIGHT; y++)                                  \
    {                                                                             \
        for (size_t x = 0; x < C_SCREEN_WIDTH; x += 4)                            \
        {                                                                         \
            const double k_x = zoomX / (double)C_SCREEN_WIDTH;                    \
            const double k_y = zoomY / (double)C_SCREEN_HEIGHT;                   \
                                                                                  \
            double x0_single = (double)x * k_x + offsetX;                         \
            double y0_single = (double)y * k_y - offsetY;                         \
                                                                                  \
            double X0[4];                                                         \
            __m256d X_vec, Y_vec;                                                 \
            X0[0] = x0_single;                                                    \
            X0[1] = x0_single + 1 * k_x;                                          \
            X0[2] = x0_single + 2 * k_x;                                          \
            X0[3] = x0_single + 3 * k_x;                                          \
            X_vec = _mm256_loadu_pd(X0);                                          \
            Y_vec = _mm256_set1_pd(y0_single);                                    \
                                                                                  \
            unsigned int N[4] = {0, 0, 0, 0};                                     \
            const int max_iteration = 100;                                        \
            double c_maxRadius = 4.0;                                             \
            __m256d X0_vec = _mm256_loadu_pd(X0);                                 \
            __m256d Y0_vec = Y_vec;                                               \
            for (unsigned int n = 0; n < max_iteration; n++)                      \
            {                                                                     \
                __m256d X2_vec = _mm256_mul_pd(X_vec, X_vec);                     \
                __m256d Y2_vec = _mm256_mul_pd(Y_vec, Y_vec);                     \
                __m256d XY_vec = _mm256_mul_pd(X_vec, Y_vec);                     \
                __m256d R2_vec = _mm256_add_pd(X2_vec, Y2_vec);                   \
                                                                                  \
                unsigned int cmp[4] = {};                                         \
                __m256d maxRadius = _mm256_set1_pd(c_maxRadius);                  \
                __m256d CMP_vec = _mm256_cmp_pd(R2_vec, maxRadius, _CMP_LE_OQ);   \
                int mask = _mm256_movemask_pd(CMP_vec);                           \
                cmp[0] = (mask & 1);                                              \
                cmp[1] = (mask & 2) >> 1;                                         \
                cmp[2] = (mask & 4) >> 2;                                         \
                cmp[3] = (mask & 8) >> 3;                                         \
                                                                                  \
                if (!mask) break;                                                 \
                                                                                  \
                __m128i N_vec = _mm_loadu_si128((__m128i*)N);                     \
                __m128i cmp_vec = _mm_loadu_si128((__m128i*)cmp);                 \
                __m128i N_result = _mm_add_epi32(N_vec, cmp_vec);                 \
                _mm_storeu_si128((__m128i*)N, N_result);                          \
                                                                                  \
                X2_vec = _mm256_sub_pd(X2_vec, Y2_vec);                           \
                X_vec = _mm256_add_pd(X2_vec, X0_vec);                            \
                                                                                  \
                __m256d temp_vec = _mm256_add_pd(XY_vec, XY_vec);                 \
                Y_vec = _mm256_add_pd(temp_vec, Y0_vec);                          \
            }                                                                     \
                                                                                  \
            ON_BUFF_(                                                                 \
                for (int i = 0; i < 4; i++)                                           \
                {                                                                     \
                    int iteration = N[i];                                             \
                    if (iteration == max_iteration)                                   \
                    {                                                                 \
                        buffer.setPixel(x + i, y, sf::Color::Black);                  \
                    }                                                                 \
                    else                                                              \
                    {                                                                 \
                        float t = (float)iteration / max_iteration;                   \
                        unsigned char r = (unsigned char)(255 * t * 4);               \
                        unsigned char g = (unsigned char)(255 * sqrt(t * M_PI));      \
                        unsigned char b = (unsigned char)(100 * (1.0f - t * t));      \
                        buffer.setPixel(x + i, y, sf::Color(r, g, b));                \
                    }                                                                 \
                }                                                                     \
            )                                                                         \
        }                                                                         \
    }                                                                             \
    PROFILE_END_("cntPxls")         
#endif                                                                            

#ifndef NAIVE_V
#ifndef SPD1_V
#ifndef AVX4
#ifndef AVX8
#ifndef TRUEAVX256
    #define COUNTPIXELS_
#endif
#endif
#endif
#endif
#endif

int main()
{
    PROFILE_SET_LIMIT_(30000)
    sf::RenderWindow window(sf::VideoMode(C_SCREEN_WIDTH, C_SCREEN_HEIGHT), "Mandelbrot");

    CREATE_BUFFER_

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

        ON_BUFF_(buffer.create(C_SCREEN_WIDTH, C_SCREEN_HEIGHT, sf::Color::Black);)

        COUNTPIXELS_

        ON_BUFF_(
                texture.loadFromImage(buffer);
                bufferSprite.setTexture(texture);
                )
        

        // FPS
        // frameCount++;
        // float currentTime = clock.getElapsedTime().asSeconds();
        // if (currentTime - lastTime >= 1.f) {
        //     float fps = frameCount / (currentTime - lastTime);
        //     printf("FPS: %.0f\n", fps);
        //     frameCount = 0;
        //     lastTime = currentTime;
        // }

        window.clear();
        ON_BUFF_(window.draw(bufferSprite);)
        window.display();
    }
    PROFILE_INFO_
    return 0;
}
