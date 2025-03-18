extern int myprintf(const char *format, ...);

int main()
{
    if (myprintf("%s %5 %b\n","qwety", 8, 11, 18) == -1)
    {
        myprintf("%s\n", "Error!");
    };
}
