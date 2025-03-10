#define M_CHECK_ERROR if (err.getErrorCode() != NO_ERR) {  }

// M_CHECK_ERROR

#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <strings.h>
#include <iostream>

const size_t lenOfTempStr = 100;

enum errors
{
    NO_ERR                   = 0,
    FILE_MP3_NOT_FOUND       = 10,
    FILE_PNG_NOT_FOUND       = 11,
    FILE_COM_NOT_FOUND       = 12,
    MUS_PLAYBACK_ERR         = 30,
    SYSTEM_ERR               = 40,
    EMPTY_FILE               = 60,
    WRONG_NUM_OF_ARGS        = 70,
    ALLOCATE_MEM_FAIL        = 80,
    NO_START_OF_CODE_SECTION = 90,
    NO_ADDRES_OF_CHANGES     = 91,
    NO_BYTES_TO_CHANGE       = 92,
    NO_NEW_BYTES             = 93

};

const char* getErrPhrase(errors error);

#define M_CATCHERR(file_name, str_pos, func_name)                                        \
    if (err != NO_ERR)                                                                   \
    {                                                                                    \
        if (strcmp(func_name, "main") != 0 || err == WRONG_NUM_OF_ARGS)                  \
            printf("%s:%d (%s) %s\n", file_name, str_pos, func_name, getErrPhrase(err)); \
        return err;                                                                      \
    }

class myError : public std::exception {
    private:
        errors errorCode;
        char fileName[20];
        int lineNum;
        mutable char tempStr[lenOfTempStr] = {0};
    
    public:
        // initialize error
        myError(errors code = NO_ERR, const char* file = nullptr, int line = 0) 
                    : errorCode(code), lineNum(line) 
        {
            if (file) 
            {
                // strncpy(fileName, file, sizeof(fileName) - 1);
                fileName[sizeof(fileName) - 1] = '\0';
            } 
            else 
            {
                fileName[0] = '\0';
            }
        }

        const char * what () const noexcept override
        {
            snprintf(tempStr, lenOfTempStr, "Error: %s, file: %s, line: %d", getErrorMessage(), fileName, lineNum);
            return tempStr;
        }

        const char* getErrorMessage() const 
        {
            switch (errorCode) 
            {
                case NO_ERR:                   return "No error";
                case FILE_MP3_NOT_FOUND:       return "MP3 file not found";
                case FILE_PNG_NOT_FOUND:       return "PNG file not found";
                case MUS_PLAYBACK_ERR:         return "Music playback error";
                case SYSTEM_ERR:               return "System error";
                case FILE_COM_NOT_FOUND:       return "COM file not found";
                case EMPTY_FILE:               return "Empty file";
                case WRONG_NUM_OF_ARGS:        return "Wrong number of arguments";
                case ALLOCATE_MEM_FAIL:        return "Allocate memory fail";
                case NO_START_OF_CODE_SECTION: return "No start of code section in config file";
                case NO_ADDRES_OF_CHANGES:     return "No address of changes in config file";
                case NO_BYTES_TO_CHANGE:       return "No bytes to change in config file";
                case NO_NEW_BYTES:             return "No new bytes in config file";
                default:                       return "Unknown error";
            }
        }

};

#endif // ERRORS_H