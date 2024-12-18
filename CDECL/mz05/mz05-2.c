#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//made by: CDECL

const char FORMAT[] = "rwxrwxrwx";

enum
{
    BASE = 8,
    NUM = sizeof(FORMAT) - 1
};

int
main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        errno = 0;
        char *endptr = NULL;
        unsigned rights = strtoul(argv[i], &endptr, BASE);
        if (errno || *endptr || endptr == argv[i]) {
            exit(1);
        }
        unsigned mask = 1 << (NUM - 1);
        for (int j = 0; j < NUM; j++) {
            if (rights & mask) {
                printf("%c", FORMAT[j]);
            } else {
                printf("-");
            }
            mask >>= 1;
        }
        printf("\n");
    }
}

