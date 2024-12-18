#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//made by: CDECL

enum CONSTS
{
    DEGREE = 10000,
    PERCENT = 100
};

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        exit(1);
    }
    double start;
    double change;
    char *endptr;
    start = strtod(argv[1], &endptr);
    start *= DEGREE;
    for (int i = 2; i < argc; i++) {
        change = strtod(argv[i], &endptr);
        start = round(start * (PERCENT + change) / PERCENT);
    }
    start /= DEGREE;
    printf("%.4lf\n", start);
    return 0;
}

