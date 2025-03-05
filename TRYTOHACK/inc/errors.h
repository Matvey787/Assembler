#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

enum errors
{
    NO_ERR             = 0,
    FILE_MP3_NOT_FOUND = 10,
    FILE_PNG_NOT_FOUND = 11,
    FILE_COM_NOT_FOUND = 12,
    MUS_PLAYBACK_ERR   = 30,
    SYSTEM_ERR         = 40,
    EMPTY_FILE         = 60,
    WRONG_NUM_OF_ARGS  = 70,
    ALLOCATE_MEM_FAIL  = 80

};

class ErrorHandler {
private:
    errors errorCode;

public:
    ErrorHandler(errors code) : errorCode(code) {}

    const char* getErrorMessage() const {
        switch (errorCode) {
            case NO_ERR:             return "No error";
            case FILE_MP3_NOT_FOUND: return "MP3 file not found";
            case FILE_PNG_NOT_FOUND: return "PNG file not found";
            case MUS_PLAYBACK_ERR:   return "Music playback error";
            case SYSTEM_ERR:         return "System error";
            case FILE_COM_NOT_FOUND: return "COM file not found";
            case EMPTY_FILE:         return "Empty file";
            case WRONG_NUM_OF_ARGS:  return "Wrong number of arguments";
            default:                 return "Unknown error";
        }
    }

    void printError() const {
        printf("Error code: %d %s", errorCode, getErrorMessage());
    }

    errors getErrorCode() const {
        return errorCode;
    }
};





#endif // ERRORS_H