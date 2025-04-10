#ifndef PROFILER
#define PROFILER

#include <stdio.h>

const size_t c_maxProfiles = 100;
const size_t c_dataLength = 100000;
struct ProfileData
{
    char name[64];
    unsigned long long startTicks;
    unsigned long long dataTicks[c_dataLength];
    size_t callCount;
};

ProfileData* getProfile(const char* name);
void profileStart(const char* name);
void profileEnd(const char* name);
void setProfileLimit(size_t limitOfCalls = c_dataLength);
void printStats();

#endif // PROFILER_H