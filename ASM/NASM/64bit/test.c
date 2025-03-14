#include <stdio.h>

extern void myprintf(const char *format, ...);

int main()
{
    myprintf("%d", 13);
    printf("\n");
}
