#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//made by: ded

int
main(int argc, char *argv[])
{
    long res_pos = 0, res_neg = 0;
    for (int i = 1; i < argc; ++i) {
        char *eptr = NULL;
        errno = 0;
        long lval = strtol(argv[i], &eptr, 10);
        if (errno || *eptr || eptr == argv[i] || (int) lval != lval) {
            exit(1);
        }
        if (lval > 0) {
            res_pos += lval;
        } else {
            res_neg += lval;
        }
    }

    printf("%ld\n%ld\n", res_pos, res_neg);
    return 0;
}
