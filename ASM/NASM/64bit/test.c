extern int myprintf(const char *format, ...);

int main()
{
    if (myprintf("%x\n", 17) == -1)
    {
        myprintf("%s\n", "Error!");
    };
}
