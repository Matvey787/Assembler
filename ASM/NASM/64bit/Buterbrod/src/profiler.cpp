#include <stdio.h>
#include <string.h>
#include <math.h>
#include <immintrin.h>

#include "profiler.h"
#include "colors.h"

#ifdef ONPROGBAR
    #define PROGBAR_(...) __VA_ARGS__
#endif
#ifndef ONPROGBAR
    #define PROGBAR_(...)
#endif
size_t profileCount = 0;
size_t dataTicksLimit = c_dataLength;

const size_t c_progressBarWidth = 20;

ProfileData profiles[c_maxProfiles];

static void progressBar(size_t calls);

ProfileData* getProfile(const char* name) 
{
    for (size_t i = 0; i < profileCount; i++)
    {
        if (strcmp(profiles[i].name, name) == 0) return &profiles[i];
    }

    if (profileCount < c_maxProfiles) 
    {
        
        size_t nameLen = strlen(name);
        size_t copyLen = (nameLen < sizeof(profiles[profileCount].name) - 1) ? nameLen : 
        sizeof(profiles[profileCount].name) - 1;

        strncpy(profiles[profileCount].name, name, copyLen);
        profiles[profileCount].name[copyLen] = '\0';

        profiles[profileCount].startTicks = 0;
        profiles[profileCount].callCount = 0;
        memset(profiles[profileCount].dataTicks, 0, sizeof(profiles[profileCount].dataTicks));
        return &profiles[profileCount++];
    }
    return nullptr;
}

void setProfileLimit(size_t limitOfCalls)
{
    if (limitOfCalls > c_dataLength)
    {
        printf("%s Warning: the number of calls is too high, it will be cut to a limit of 100000%s\n", BRIGHT_MAGENTA_, EXITCOLOR_);
        fflush(stdout);
        dataTicksLimit = c_dataLength;
    }
    else
    {
        dataTicksLimit = limitOfCalls;
        printf("%s The number of tests (%lu) has been successfully set.%s\n", BRIGHT_MAGENTA_, limitOfCalls, EXITCOLOR_);
        fflush(stdout);
    }
    
}

void profileStart(const char* name)
{
    ProfileData* data = getProfile(name);
    if (data)
    {
        PROGBAR_(progressBar(data->callCount);)
        if (data->callCount < dataTicksLimit)
        {
            data->startTicks = __rdtsc();
        }
    }
}

void profileEnd(const char* name)
{
    unsigned long long endTicks = __rdtsc();
    ProfileData* data = getProfile(name);
    size_t testIndex = data->callCount;
    if (data)
    {
        if (testIndex < dataTicksLimit)
        {
            data->dataTicks[testIndex] = endTicks - data->startTicks;
            data->callCount++;
        } 
    }
}

void printStats()
{
    printf("\n===== Profiler Stats =====\n");
    
    for (size_t profIndex = 0; profIndex < profileCount; profIndex++)
    {
        printf("%s Profile: %s %s\n", YELLOW_, profiles[profIndex].name, EXITCOLOR_);

        // average of ticks calculation
        unsigned long long ticksAverage = 0;
        size_t numOfTests = profiles[profIndex].callCount;

        for (size_t testIndex = 0; testIndex < numOfTests; testIndex++)
        {
            ticksAverage += profiles[profIndex].dataTicks[testIndex];
        }

        ticksAverage /= numOfTests;

        // dispersion calculation
        unsigned long long dispersion = 0;
        for (size_t j = 0; j < numOfTests; j++)
        {
            dispersion += ((profiles[profIndex].dataTicks[j] - ticksAverage) * 
                           (profiles[profIndex].dataTicks[j] - ticksAverage));
        }
        //--numOfTests;
        long double doubleD = sqrt(dispersion / numOfTests);
        
        printf("%s Ticks average between start/end: %s %llu +- %llu\n", GREEN_, EXITCOLOR_,  
                                                        ticksAverage, (unsigned long long)doubleD);
        printf("%s Calls: %s %lu\n", GREEN_, EXITCOLOR_, profiles[profIndex].callCount);
    }
}

static void progressBar(size_t calls) {
    double progress = (double)calls / dataTicksLimit; 
    size_t progressInProgressBar = progress * c_progressBarWidth; 

    printf("\r[%s", BRIGHT_GREEN_); 
    for (size_t i = 0; i < c_progressBarWidth; i++) {
        if (i < progressInProgressBar) {
            printf("█"); 
        } else {
            printf(" "); 
        }
    }
    printf("%s] %d%%", EXITCOLOR_, (int)(progress * 100)); 
    fflush(stdout); 
}
