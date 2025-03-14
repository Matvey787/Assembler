
#include "CRACKalka.h"
#include "constants.h"
#include "errors.h"

#include <assert.h>

#define M_CATCHERR(err_, file_name, str_pos, func_name)                                        \
    if (err_ != NO_ERR)                                                                   \
    {                                                                                    \
        if (strcmp(func_name, "main") != 0 || err == WRONG_NUM_OF_ARGS)                  \
            printf("%s:%d (%s) %s\n", file_name, str_pos, func_name, getErrPhrase(err)); \
        return err_;                                                                      \
    }

// We go through the *.com file and replace byte after jne with 04 - address of success.
int main(int argc, char const *argv[])
{
    errors err = NO_ERR;
    if (argc != 3)
    {
        err = WRONG_NUM_OF_ARGS;
    }
    M_CATCHERR(__FILE__, __LINE__, __func__); // FIXME 

    err = procComFile(argv[1], argv[2]);
    M_CATCHERR(__FILE__, __LINE__, __func__);
    // err = playMusic("sigma.mp3"); // play music and call func which draws window  
    err = playMusicAndDrawPictureSfml("sigma.mp3", "duck.png");
    M_CATCHERR(__FILE__, __LINE__, __func__);
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

    err = readFileMmap(&(comBuff.data), comFileName, &(comBuff.size)); // FIXME unmap
    M_CATCHERR(__FILE__, __LINE__, __func__);

    err = readFile(&(confBuff.data), configFileName, &(confBuff.size));
    M_CATCHERR(__FILE__, __LINE__, __func__);

    printf("File content:\n-------------------- start --------------------\n");
    for (size_t i = 0; i < comBuff.size; ++i)
    {
        if (i % 16 == 0 && i != 0)
        {
            printf("\n");
        }

        if (comBuff.data[i] >= 32 && comBuff.data[i] <= 126) // area of symbols which are printable
        {
            printf("%lc ", comBuff.data[i]);
        }
        else
        {
            printf("%x ", comBuff.data[i]);
        }
    }
    printf("\n-------------------- end --------------------\n");

    err = crackFile(&comBuff, &confBuff);
    M_CATCHERR(__FILE__, __LINE__, __func__);

    // rewrite file
    //err = writeFile(comBuff.data, comFileName, comBuff.size);
    M_CATCHERR(__FILE__, __LINE__, __func__);

    if (err == NO_ERR)
        printf("File successfully cracked!\n");

    //free(comBuff.data);
    free(confBuff.data);
    return err;
}

errors crackFile(buffer* comBuff, buffer* confBuff)
{
    assert(comBuff != nullptr);
    assert(confBuff != nullptr);
    
    // parsing config file
    errors err = NO_ERR;
    bool skipComm = false;
    bool codeSectionFound = false;
    
    for (size_t offset = 0; offset < confBuff->size; ++offset)
    {
        if (skipComm && confBuff->data[offset] != '\n')
            continue;

        if (confBuff->data[offset] == '#')
        {
            skipComm = true;
        }readFileMmap
        else if (confBuff->data[offset] == '\n')
        {
            skipComm = false;
        }
        else
        {   // if it's command line
            err = parseString(confBuff, comBuff, &offset, &codeSectionFound);
            M_CATCHERR(__FILE__, __LINE__, __func__);
        }
    }
    return NO_ERR;
}

errors parseString(buffer* confBuff, buffer* comBuff, size_t* offset, bool* codeSectionFound)
{
    assert(confBuff != nullptr);
    assert(offset != nullptr);

    int readedChars = 0;
    static unsigned int codeSection = 0; // offset of code section in com file

    if (!(*codeSectionFound))
    {
        if (!sscanf((const char*)(confBuff->data + *offset), "%xh%n", &codeSection, &readedChars))
        {
            return NO_START_OF_CODE_SECTION;
        }
        *offset += (size_t)readedChars; // skip number
        *codeSectionFound = true;
        return NO_ERR;
    }

    unsigned int addressOfStart = 0;   // start address of bytes to change in com file
    unsigned int bytesToChange  = 0;   // number of bytes to change in com file

    if (!sscanf((const char*)(confBuff->data + *offset), "%xh%n", &addressOfStart, &readedChars))
    {
        return NO_ADDRES_OF_CHANGES;
    }
    *offset += (size_t)readedChars; // skip number and ' '
    ++*offset; // skip ' '
    printf("bytes to change: %x\n", addressOfStart);
    if (!sscanf((const char*)(confBuff->data + *offset), "%u%n", &bytesToChange, &readedChars))
    {
        return NO_BYTES_TO_CHANGE;
    }
    printf("bytes to change: %d\n", bytesToChange);
    *offset += (size_t)readedChars; // skip digit and ' '
    ++*offset; // skip ' '
    
    unsigned int COMFileoffset = addressOfStart - codeSection;
    for (size_t j = 0; j < bytesToChange; ++j)
    {
        unsigned int byte = 0;
        if (!sscanf((const char*)(confBuff->data + *offset), "%x%n", &byte, &readedChars))
        {
            return NO_NEW_BYTES;
        }
        printf("length: %d %d\n", comBuff->size, COMFileoffset + j);
        comBuff->data[COMFileoffset + j] = (unsigned char)byte;

        *offset += (size_t)readedChars; // skip readed byte
    }
    return NO_ERR;
}
