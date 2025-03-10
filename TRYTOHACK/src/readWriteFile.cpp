#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "CRACKalka.h"
#include "errors.h"

errors readFileMmap(unsigned char** buffer, const char* file_name, size_t* numOfSymbols, size_t* numOfStrs) {
    assert(file_name != nullptr);

    // open file for reading and writing
    int fileDescriptor = open(file_name, O_RDWR);
    if (fileDescriptor == -1) 
    {
        return FILE_COM_NOT_FOUND;
    }

    // get file info
    struct stat file_info = {0};
    if (fstat(fileDescriptor, &file_info) == -1) 
    {
        close(fileDescriptor);
        return FILE_COM_NOT_FOUND;
    }

    size_t local_numOfSymbols = (size_t)file_info.st_size;

    // check if file is empty
    if (local_numOfSymbols == 0) 
    {
        *buffer = nullptr;

        if (numOfSymbols) *numOfSymbols = 0;
        if (numOfStrs) *numOfStrs = 0;

        close(fileDescriptor);
        return EMPTY_FILE;
    }

    void* mapped = mmap(NULL, local_numOfSymbols, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    if (mapped == MAP_FAILED)
    {
        close(fileDescriptor);
        return FILE_COM_NOT_FOUND;
    }

    // buffer is pointing to mapped memory
    *buffer = (unsigned char*)mapped;

    // count number of strings if it's needed
    if (numOfStrs != nullptr) 
    {
        *numOfStrs = 0;
        for (size_t i = 0; i < local_numOfSymbols; i++) 
        {
            if ((*buffer)[i] == '\n') 
            {
                (*numOfStrs)++;
            }
        }
        // and the last string
        if (local_numOfSymbols > 0 && (*buffer)[local_numOfSymbols - 1] != '\n') 
        {
            (*numOfStrs)++;
        }
    }

    // size of file
    if (numOfSymbols != nullptr) {
        *numOfSymbols = local_numOfSymbols;
        printf("size of file %u", local_numOfSymbols);
    }

    // close descriptor
    close(fileDescriptor);

    return NO_ERR;
}

errors readFile(unsigned char** buffer, const char* file_name, size_t* numOfSymbols,
                                                               size_t* numOfStrs)
{
    assert(buffer       != nullptr);
    assert(file_name    != nullptr);

    // open file for read
    FILE* rFile = fopen(file_name, "rb");

    // if the file can't be opened, return an error
    if (rFile == nullptr) {
        //throw myError(FILE_COM_NOT_FOUND, __FILE__, __LINE__);
        return FILE_COM_NOT_FOUND;
    }

    // find size of file
    fseek(rFile, 0, SEEK_END);
    size_t local_numOfSymbols = (size_t)ftell(rFile);
    fseek(rFile, 0, SEEK_SET);

    // read text from file
    *buffer = (unsigned char*)calloc(local_numOfSymbols + 1, sizeof(char));

    // if the memory can't be allocated, return an error
    if (*buffer == nullptr) {
        //myError(ALLOCATE_MEM_FAIL, __FILE__, __LINE__);
        return ALLOCATE_MEM_FAIL;
    }

    // read the file into the buffer
    fread(*buffer, sizeof(char), local_numOfSymbols, rFile);

    // add '\n' at the end of the buffer if it doesn't exist
    if ((*buffer)[local_numOfSymbols - 1] != '\n') {
        (*buffer)[local_numOfSymbols] = '\n';
        ++(local_numOfSymbols);
    }

    // find number of strings
    if (numOfStrs != nullptr)
    {
        for (size_t i = 0; i < local_numOfSymbols; i++)
            if ((*buffer)[i] == '\n')
                ++(*numOfStrs);
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