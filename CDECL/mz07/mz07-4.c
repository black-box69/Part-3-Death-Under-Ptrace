#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

//made by: CDECL

enum
{
    ARGC_STD = 2
};

int
main(int argc, char **argv)
{
    if (ARGC_STD != argc) {
        exit(1);
    }
    int fd;
    if ((fd = open(argv[1], O_RDWR, 0666)) == -1) {
        exit(1);
    }
    struct stat st;
    if (stat(argv[1], &st) == -1) {
        exit(1);
    }
    long long size = st.st_size;
    int x = 0, byte_num = 0, offset = 0;
    char buf;
    unsigned char mask;
    while (scanf("%d", &x) != -1) {
        if (x > 0 && --x < size * CHAR_BIT) {
            byte_num = x / CHAR_BIT;
            offset = x % CHAR_BIT;
            if (lseek(fd, byte_num, SEEK_SET) == -1) {
                exit(1);
            }
            if (read(fd, &buf, sizeof(buf)) == -1) {
                exit(1);
            }
            mask = 1 << offset;
            buf |= mask;
            if (lseek(fd, byte_num, SEEK_SET) == -1) {
                exit(1);
            }
            if (write(fd, &buf, sizeof(buf)) == -1) {
                exit(1);
            }
        } else if (x < 0 && -x <= size * CHAR_BIT) {
            x = -x;
            x--;
            byte_num = x / CHAR_BIT;
            offset = x % CHAR_BIT;
            if (lseek(fd, byte_num, SEEK_SET) == -1) {
                exit(1);
            }
            if (read(fd, &buf, sizeof(buf)) == -1) {
                exit(1);
            }
            mask = ~(0) ^ (1 << offset);
            buf &= mask;
            if (lseek(fd, byte_num, SEEK_SET) == -1) {
                exit(1);
            }
            if (write(fd, &buf, sizeof(buf)) == -1) {
                exit(1);
            }
        }
    }
    if (close(fd) == -1) {
        exit(1);
    }
}

