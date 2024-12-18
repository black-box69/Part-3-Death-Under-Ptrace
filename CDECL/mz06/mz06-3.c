#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

struct s1
{
    char f1;
    long long f2;
    char f3;
};

struct s2
{
    long long f2;
    char f1;
    char f3;
};

size_t
compactify(void *ptr, size_t size)
{
    size_t res = 0;
    struct s1 cur;
    struct s2 new_cur;
    struct s1 *large = ptr;
    struct s2 *small = ptr;
    for (int i = 0; i < size / sizeof(cur); i++) {
        cur = *large;
        new_cur.f1 = cur.f1;
        new_cur.f2 = cur.f2;
        new_cur.f3 = cur.f3;
        *small = new_cur;
        large++;
        small++;
    }
    res = size * sizeof(new_cur) / sizeof(cur);
    return res;
}

