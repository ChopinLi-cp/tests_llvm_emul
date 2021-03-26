#include <stdio.h>
static int d;

int shift(int c, int b)
{
    int a = 12;
    d = a << 4;
    d += c - b;
    printf("The result is %d.\n", d);
    return d;
}

int main()
{
    int e = 13;
    int f = 14;
    return shift(e, f);
}
