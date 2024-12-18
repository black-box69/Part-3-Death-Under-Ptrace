#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <errno.h>

//made by: ded

enum
{
    BYTE_MASK = 0x1,
    SIZEOF_INT = 4
};

int
main(int argc, char **argv)
{
    int fd_1 = open(argv[1], O_RDONLY);
    int fd_2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);

    if (fd_1 == -1 || fd_2 == -1) {
        exit(errno);
    }

    long long mod = 0;
    char *eptr = NULL;
    errno = 0;
    mod = strtoll(argv[3], &eptr, 10);
    if (errno || *eptr || eptr == argv[3] || (long long) mod != mod) {
        exit(1);
    }
    long long iter = 1;
    long long result = 0;
    unsigned char read_byte = 0;

    while (read(fd_1, &read_byte, sizeof(read_byte)) > 0) {
        for (int i = 0; i < 8; ++i) {
            result += (iter * iter) % mod;
            result = result % mod;
            if (read_byte & BYTE_MASK) {
                int int_result = (int) result;
                write(fd_2, &int_result, SIZEOF_INT);
            }
            iter++;
            read_byte = read_byte >> 1;
        }
    }

    close(fd_1);
    close(fd_2);
    return 0;
}
