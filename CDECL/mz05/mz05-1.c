#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

//made by: CDECL

enum
{
    KIBIBYTE = 1024
};

int
main(int argc, char **argv)
{
    unsigned long long res = 0;
    for (int i = 1; i < argc; i++) {
        struct stat cur;
        if (lstat(argv[i], &cur) >= 0 && cur.st_nlink == 1 && (cur.st_size & (KIBIBYTE - 1)) == 0 &&
            S_ISREG(cur.st_mode)) {
            res += cur.st_size;
        }
    }
    printf("%llu\n", res);
}

