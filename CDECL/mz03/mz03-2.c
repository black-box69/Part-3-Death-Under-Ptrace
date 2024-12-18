#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//made by: CDECL

enum CONSTS
{
    BASE = 10
};

int
main(int argc, char *argv[])
{
    long long plus = 0, minus = 0;
    char *endptr;
    long long tmp = 0;
    errno = 0;
    for (int i = 1; i < argc; i++) {
        tmp = strtol(argv[i], &endptr, BASE);
        if (errno || *endptr || endptr == argv[i] || (int) tmp != tmp) {
            fprintf(stderr, "Ошибка при обработке данных: ");
            exit(1);
        }
        if (tmp > 0) {
            plus += tmp;
        } else {
            minus += tmp;
        }
    }
    printf("%lld\n%lld\n", plus, minus);
    return 0;
}

