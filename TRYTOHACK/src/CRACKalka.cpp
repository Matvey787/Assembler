
#include "CRACKalka.h"
#include "constants.h"
#include "errors.h"

#include <assert.h>


// We go through the *.com file and replace byte after jne with 04 - address of success.
int main(int argc, char const *argv[])
{
    errors err = NO_ERR;
    if (argc != 2)
    {
        printf("Wrong number of arguments!\n");
        return WRONG_NUM_OF_ARGS;
    }

    err = procComFile(argv[1]);

    if (err != NO_ERR)
    {
        printf("Something go wrong during processing com file!\n");
        return err;
    }
    err = playMusic("sigma.mp3"); // play music and call func which draws window  
    return err;
}

errors procComFile(const char* comFileName)
{
    assert(comFileName != nullptr);

    errors err = NO_ERR;
    FILE* file = fopen(comFileName, "r+");

    if (file == nullptr) 
    {
        return FILE_COM_NOT_FOUND;
    }

    unsigned char buffer[KByte] = {0};
    size_t numOfChars = 0;

    err = readFile(file, buffer, &numOfChars);
    crackFile(buffer, numOfChars);
    

    // rewrite file
    fseek(file, 0, SEEK_SET);
    for (size_t i = 0; i < numOfChars; i++)
    {
        putc(buffer[i], file);
    }

    if (err != NO_ERR)
        printf("File successfully cracked!\n");
    
    fclose(file);
    return err;
}

void crackFile(unsigned char* buffer, size_t numOfChars)
{
    assert(buffer != nullptr);

    int stopCounter = 0;
    for (size_t i = 0; i < numOfChars; i++)
    {
        //printf("%x\n", buffer[i]);
        if (isalpha(buffer[i]))
        {
            ++stopCounter;
        }
        else
        {
            stopCounter = 0;
        }
        if (stopCounter == 3)
            break;

        if (buffer[i] == JNE_ASCII_CODE)
        {//              ^------ ascii code of jne (75 in hex)
            //printf("---------------- %x\n", buffer[i]);
            buffer[i + 1] = 4; // 4 - address of success
        }
    }
}

errors readFile(FILE* file, unsigned char* buffer, size_t* numOfChars)
{
    assert(file != nullptr);
    assert(buffer != nullptr);

    *numOfChars = fread(buffer, sizeof(*buffer), KByte, file);

    if (numOfChars == 0) 
    {
        printf("File is empty!\n");
        return EMPTY_FILE;
    }

    printf("File content:\n-------------------- start --------------------\n");
    for (size_t i = 0; i < *numOfChars; ++i) 
    {
        if (i % 16 == 0 && i != 0)
        {
            printf("\n");
        }

        if (buffer[i] >= 32 && buffer[i] <= 126)
        {
            printf("%lc ", buffer[i]);
        }
        else
        {
            printf("%x ", buffer[i]);
        }
    }
    printf("\n-------------------- end --------------------\n");
    return NO_ERR;
}



