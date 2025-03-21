extern int myprintf(const char *format, ...);

int main()
{
    if (myprintf("%o %x %b %x %x %x %x\n", 9, 2, 10, 4, 5, 6, 256) == -1)
    {
        myprintf("%s\n", "Error!");
    };
}
