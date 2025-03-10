#include "errors.h"

const char* getErrPhrase(errors error)
{
    switch (error) 
    {
        case NO_ERR:                   return "No error";
        case FILE_MP3_NOT_FOUND:       return "MP3 file not found";
        case FILE_PNG_NOT_FOUND:       return "PNG file not found";
        case MUS_PLAYBACK_ERR:         return "Music playback error";
        case SYSTEM_ERR:               return "System error";
        case FILE_COM_NOT_FOUND:       return "COM file not found";
        case EMPTY_FILE:               return "Empty file";
        case WRONG_NUM_OF_ARGS:        return "Wrong number of arguments! Example: ./run config comFile.com\n";
        case ALLOCATE_MEM_FAIL:        return "Allocate memory fail";
        case NO_START_OF_CODE_SECTION: return "No start of code section in config file";
        case NO_ADDRES_OF_CHANGES:     return "No address of changes in config file";
        case NO_BYTES_TO_CHANGE:       return "No bytes to change in config file";
        case NO_NEW_BYTES:             return "No new bytes in config file";
        default:                       return "Unknown error";
    }
}
