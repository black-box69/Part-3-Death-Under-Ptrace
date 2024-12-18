#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

int
comp(const void *v1, const void *v2)
{
    long long a = *(int *) v1;
    long long b = *(int *) v2;
    if (!(a & 1) && (b & 1)) {
        return -1;
    } else if ((a & 1) && !(b & 1)) {
        return 1;
    } else if (!(a & 1) && !(b & 1)) {
        if (a > b) {
            return 1;
        } else {
            return -1;
        }
    } else {
        if (b > a) {
            return 1;
        } else {
            return -1;
        }
    }
}

void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, sizeof(*data), comp);
}

