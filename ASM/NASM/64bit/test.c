extern void myprintf(const char *format, ...);

int main()
{
    myprintf("%b \\n %c %o %x %d %x", 11, 'a', -11, 423, 189, 1233);
}
