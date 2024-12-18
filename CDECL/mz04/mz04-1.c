#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

//made by: CDECL

enum
{
    BYTE_MASK = 0xff,
    H_BYTE_MASK = 0x0f,
    BUF_SIZE = 4,
    SYS_CHAR_BIT = 12
};

unsigned
conv(unsigned digit, unsigned mask, unsigned mov)
{
    return (digit & (mask << mov)) >> mov;
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        exit(1);
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        exit(1);
    }
    unsigned digit = 0;
    while (scanf("%u", &digit) == 1) {
        char buf[BUF_SIZE] = {0};
        buf[0] = conv(digit, H_BYTE_MASK, SYS_CHAR_BIT + CHAR_BIT);
        buf[1] = conv(digit, BYTE_MASK, SYS_CHAR_BIT);
        buf[2] = conv(digit, H_BYTE_MASK, CHAR_BIT);
        buf[3] = conv(digit, BYTE_MASK, 0);
        if (write(fd, buf, BUF_SIZE) == -1) {
            exit(1);
        }
    }
    close(fd);
}

