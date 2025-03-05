
#include "CRACKalka.h"
#include "constants.h"
#include "errors.h"

#include <assert.h>

// We go through the *.com file and replace byte after jne with 04 - address of success.
int main(int argc, char const *argv[])
{
    errors err = NO_ERR;
    if (argc != 3)
    {
        printf("Wrong number of arguments! Example: ./run config comFile.com\n");
        return WRONG_NUM_OF_ARGS;
    }

    err = procComFile(argv[1], argv[2]);

    if (err != NO_ERR)
    {
        printf("Something go wrong during processing com file!\n");
        return err;
    }
    err = playMusic("sigma.mp3"); // play music and call func which draws window  
    return err;
}

errors procComFile(const char* configFileName, const char* comFileName)
{
    assert(configFileName != nullptr);
    assert(comFileName != nullptr);

    errors err = NO_ERR;

    // for com file
    buffer comBuff = {0};

    // for config file 
    buffer confBuff = {0};

    err = readFile(&(comBuff.data),  comFileName,    &(comBuff.size));
    err = readFile(&(confBuff.data), configFileName, &(confBuff.size));


    printf("File content:\n-------------------- start --------------------\n");
    for (size_t i = 0; i < confBuff.size; ++i) 
    {
        if (i % 16 == 0 && i != 0)
        {
            printf("\n");
        }

        if (confBuff.data[i] >= 32 && confBuff.data[i] <= 126)
        {
            printf("%lc ", confBuff.data[i]);
        }
        else
        {
            printf("%x ", confBuff.data[i]);
        }
    }
    printf("\n-------------------- end --------------------\n");

    crackFile(&comBuff, &confBuff);
    // rewrite file
    err = writeFile(comBuff.data, comFileName, comBuff.size);

    if (err != NO_ERR)
        printf("File successfully cracked!\n");

    free(comBuff.data);
    free(confBuff.data);
    return err;
}

void crackFile(buffer* comBuff, buffer* confBuff)
{
    assert(comBuff != nullptr);
    assert(confBuff != nullptr);

    // parsing config file
    size_t codeSection = 0; // offset of code section in com file
    bool skipComm = false;
    bool codeSectionFound = false;
    printf("here %lu\n", confBuff->size);
    for (size_t i = 0; i < confBuff->size; ++i)
    {
        printf("Before skip %c\n", confBuff->data[i]);
        // getchar();
        if (skipComm && confBuff->data[i] != '\n')
            continue;
        printf("After skip: %c\n", confBuff->data[i]);

        if (confBuff->data[i] == '#')
            skipComm = true;
        else if (confBuff->data[i] == '\n')
            skipComm = false;
        else
        {
            if (!codeSectionFound)
            {
                printf("char %c", confBuff->data[i]);
                int readedChars = sscanf((const char*)(confBuff->data + i), "%xh", &codeSection);
                printf("----------100--------- %xh\n", codeSection);
                i += 4;
                codeSectionFound = true;
                continue;
            }
            size_t addressOfStart = 0;   // start address of bytes to change in com file
            size_t bytesToChange  = 0;   // number of bytes to change in com file

            int readedChars = sscanf((const char*)(confBuff->data + i), "%lxh", &addressOfStart);
            printf("------------------- %lxh\n", addressOfStart);
            i += 4;

            readedChars = sscanf((const char*)(confBuff->data + i), "%lu", &bytesToChange);
            i += 2;
            printf("ttttt %c %c\n", *(confBuff->data + i), *(confBuff->data + i + 1));
            size_t offset = addressOfStart - codeSection;
            printf("offset: %u %u\n", offset, bytesToChange);
            for (size_t j = 0; j < bytesToChange; ++j)
            {
                unsigned char byte = 0;

                sscanf((const char*)(confBuff->data + i), "%d", &byte);
                printf("byte: %d\n", byte);
                comBuff->data[offset + j] = byte;

                i += 3; // skip readed byte and ' '
            }
        }
    }
}
