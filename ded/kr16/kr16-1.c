#include <stdio.h>
#include <stdlib.h>

enum
{
    FREE = 0,
    USER_DATA = 1,
    SYSTEM_DATA = 2,
    BAD = 3
};

int get_2bit(unsigned long long bitset) {
    int x = (bitset & 0x1) + (bitset & 0x2);
    return x;
}

int
main(int argc, char **argv)
{
    unsigned long long tmp = 0;
    unsigned long long res = 0;
    while (scanf("%llx", &tmp) > 0) {
        for (int i = 0; i < 32; i++) {
            int x = get_2bit(tmp);
            if (x == FREE) {
                res++;
            }
            tmp = tmp >> 2;
        }
    }

    printf("%llu\n", res);
}