#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "CRACKalka.h"
#include "errors.h"

errors readFile(unsigned char** buffer, const char* file_name, size_t* numOfSymbols,
                                                               size_t* numOfStrs)
{
    assert(buffer       != nullptr);
    assert(file_name    != nullptr);

    // open file for read
    FILE* rFile = fopen(file_name, "rb");

    // if the file can't be opened, return an error
    if (rFile == nullptr)
        return FILE_COM_NOT_FOUND;

    // find size of file
    fseek(rFile, 0, SEEK_END);
    size_t local_numOfSymbols = (size_t)ftell(rFile);
    fseek(rFile, 0, SEEK_SET);

    // read text from file
    *buffer = (unsigned char*)calloc(local_numOfSymbols, sizeof(char));

    // if the memory can't be allocated, return an error
    if (*buffer == nullptr)
        return ALLOCATE_MEM_FAIL;

    // read the file into the buffer
    fread(*buffer, sizeof(char), local_numOfSymbols, rFile);

    // add '\n' at the end of the buffer if it doesn't exist
    if ((*buffer)[local_numOfSymbols - 1] != '\n')
    {
        *buffer = (unsigned char*)realloc(*buffer, sizeof(unsigned char) * 
                                                                        (local_numOfSymbols + 1));
        (*buffer)[local_numOfSymbols] = '\n';
        ++(local_numOfSymbols);
    }

    // find number of strings
    if (numOfStrs != nullptr)
    {
        for (size_t i = 0; i < *numOfSymbols; i++) if ((*buffer)[i] == '\n') ++(*numOfStrs);
    }
        

    if (numOfSymbols != nullptr)
        *numOfSymbols = local_numOfSymbols;
    

    fclose(rFile);
    return NO_ERR;
}

errors writeFile(const unsigned char* buffer, const char* file_name, size_t numOfSymbols)
{
    assert(buffer       != nullptr);
    assert(file_name    != nullptr);

    if (numOfSymbols == 0)
    {
        return NO_ERR;
    }

    // open file for write
    FILE* wFile = fopen(file_name, "wb");

    // if the file can't be opened, return an error
    if (wFile == nullptr)
        return FILE_COM_NOT_FOUND;

    // write text from buffer to file
    fwrite(buffer, sizeof(char), numOfSymbols, wFile);

    fclose(wFile);

    return NO_ERR;
}