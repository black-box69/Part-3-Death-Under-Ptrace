#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

//made by: CDECL

enum
{
    BASE = 10
};

int
main(int argc, char **argv)
{
    if (argc != 3) {
        exit(1);
    }
    char *endptr = NULL;
    int N = strtol(argv[2], &endptr, BASE);
    if (errno || *endptr || endptr == argv[2]) {
        exit(1);
    }
    if (N > 1) {
        int fd = open(argv[1], O_RDWR, 0777);
        if (fd == EOF) {
            exit(1);
        }
        double prev;
        double cur;
        ssize_t checker;
        checker = read(fd, &prev, sizeof(prev));
        if (checker == 0) {
            exit(0);
        } else if (checker == -1) {
            exit(1);
        }
        for (int i = 1; i < N; i++) {
            checker = read(fd, &cur, sizeof(cur));
            if (checker == 0) {
                exit(0);
            } else if (checker == -1) {
                exit(1);
            }
            if (lseek(fd, -(off_t) sizeof(cur), SEEK_CUR) == -1) {
                exit(1);
            }
            cur = cur - prev;
            if (write(fd, &cur, sizeof(cur)) == -1) {
                exit(1);
            }
            prev = cur;
        }
        close(fd);
    }
    return 0;
}

