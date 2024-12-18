#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

//made by: ded

const double FULL_PERCENT = 100.0;
const double ROUND_CONST = 10000.0;

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        exit(1);
    }

    char *eptr = NULL;
    errno = 0;
    double dval = strtold(argv[1], &eptr);
    if (errno || *eptr || eptr == argv[2] || (double) dval != dval) {
        exit(1);
    }

    double res = dval * ROUND_CONST;

    for (int i = 2; i < argc; ++i) {
        eptr = NULL;
        errno = 0;
        dval = strtold(argv[i], &eptr);
        if (errno || *eptr || eptr == argv[i] || (double) dval != dval) {
            exit(1);
        }

        res *= (1 + dval / FULL_PERCENT);
        res = round(res);
    }

    res /= ROUND_CONST;

    printf("%.4f\n", res);
    return 0;
}
