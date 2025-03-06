
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
    // err = playMusic("sigma.mp3"); // play music and call func which draws window  
    err = playMusicAndDrawPictureSfml("sigma.mp3", "duck.png");
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

    readFile(&(comBuff.data),  comFileName,    &(comBuff.size), nullptr);
    readFile(&(confBuff.data), configFileName, &(confBuff.size), nullptr);
    

    printf("File content:\n-------------------- start --------------------\n");
    for (size_t i = 0; i < comBuff.size; ++i) 
    {
        if (i % 16 == 0 && i != 0)
        {
            printf("\n");
        }

        if (comBuff.data[i] >= 32 && comBuff.data[i] <= 126)
        {
            printf("%lc ", comBuff.data[i]);
        }
        else
        {
            printf("%x ", comBuff.data[i]);
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

errors crackFile(buffer* comBuff, buffer* confBuff)
{
    assert(comBuff != nullptr);
    assert(confBuff != nullptr);
    printf("crackFile\n");
    // parsing config file
    unsigned int codeSection = 0; // offset of code section in com file
    bool skipComm = false;
    bool codeSectionFound = false;
    for (size_t i = 0; i < confBuff->size; ++i)
    {
        if (skipComm && confBuff->data[i] != '\n')
            continue;

        if (confBuff->data[i] == '#')
            skipComm = true;
        else if (confBuff->data[i] == '\n')
            skipComm = false;
        else
        {
            if (!codeSectionFound)
            {
                if (!sscanf((const char*)(confBuff->data + i), "%xh", &codeSection))
                    return NO_START_OF_CODE_SECTION;
                i += 4; // skeep 4 symbols of number
                codeSectionFound = true;
                continue;
            }
            unsigned int addressOfStart = 0;   // start address of bytes to change in com file
            unsigned int bytesToChange  = 0;   // number of bytes to change in com file

            if (!sscanf((const char*)(confBuff->data + i), "%xh", &addressOfStart))
                return NO_ADDRES_OF_CHANGES;
            i += 5; // skeep 4 symbols of number and ' '

            if (!sscanf((const char*)(confBuff->data + i), "%u", &bytesToChange))
                return NO_BYTES_TO_CHANGE;
            i += 2; // skeep digit and ' '

            unsigned int offset = addressOfStart - codeSection;
            for (size_t j = 0; j < bytesToChange; ++j)
            {
                unsigned int byte = 0;
                if (!sscanf((const char*)(confBuff->data + i), "%x", &byte))
                    return NO_NEW_BYTES;
                comBuff->data[offset + j] = (unsigned char)byte;

                i += 3; // skip readed byte and ' '
            }
        }
    }
    return NO_ERR;
}
