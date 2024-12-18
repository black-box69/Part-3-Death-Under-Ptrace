#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

//made by: CDECL

enum
{
    NORMAL_ARGC = 2
};

int
open_safe(char *path, int flags)
{
    int fd = open(path, flags, 0600);
    if (fd == -1) {
        exit(1);
    }
    return fd;
}

ssize_t
read_safe(int fd, long long *dst)
{
    ssize_t check = read(fd, dst, sizeof(*dst));
    if (check == -1) {
        exit(1);
    }
    return check;
}

ssize_t
write_safe(int fd, long long *src)
{
    ssize_t check = write(fd, src, sizeof(*src));
    if (check != sizeof(*src)) {
        exit(1);
    }
    return check;
}

int
main(int argc, char **argv)
{
    long long cur = 0;
    long long min = LLONG_MAX;
    int flag = 0;
    off_t pos = 0;
    if (argc != NORMAL_ARGC) {
        exit(1);
    }
    int fd = open_safe(argv[1], O_RDWR);
    ssize_t checker = 0;
    while ((checker = read_safe(fd, &cur))) {
        if (checker != sizeof(cur)) {
            exit(1);
        }
        if (cur < min || !flag) {
            min = cur;
            flag = 1;
            pos = lseek(fd, 0, SEEK_CUR);
            if (pos == -1) {
                exit(1);
            }
        }
    }
    if (flag) {
        checker = lseek(fd, pos - sizeof(min), SEEK_SET);
        if (checker == -1) {
            exit(1);
        }
        min = -(unsigned long long) min;
        write_safe(fd, &min);
    }
    close(fd);
    return 0;
}

