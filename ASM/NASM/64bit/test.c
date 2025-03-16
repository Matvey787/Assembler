extern void myprintf(const char *format, ...);

int main()
{
    myprintf("\n %o %b %c %d %d %d", 47, 11, 'Q', 4124, 423, 189);
}
