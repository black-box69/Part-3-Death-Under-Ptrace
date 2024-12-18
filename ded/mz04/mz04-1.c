#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

//made by: ded

enum
{
    BYTES_CHAR = 4,
    BUF_SIZE = 4,
    TWO_BYTES_MASK = 0xff,
    ONE_BYTE_MASK = 0xf
};

int
main(int argc, char **argv)
{
    int file_fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (file_fd == -1) {
        exit(errno);
    }
    unsigned input = 0;
    int err_check = 0;

    errno = 0;
    while ((err_check = scanf("%d", &input)) > 0) {
        if (err_check == -1) {
            close(file_fd);
            exit(errno);
        }
        int i = 0;
        char buf[BUF_SIZE];
        for (int j = 0; j < 2; j++) {
            buf[BUF_SIZE - i - 1] = input & TWO_BYTES_MASK;
            input = input >> (2 * BYTES_CHAR);
            ++i;
            buf[BUF_SIZE - i - 1] = 0 | (input & ONE_BYTE_MASK);
            input = input >> BYTES_CHAR;
            ++i;
        }
        errno = 0;
        err_check = write(file_fd, buf, sizeof(input));
        if (err_check == -1) {
            close(file_fd);
            exit(errno);
        }
        errno = 0;
    }
    err_check = close(file_fd);
    if (err_check == -1) {
        exit(errno);
    }
    return 0;
}
