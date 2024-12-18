#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

//made by: CDECL

enum
{
    SUPPOSED_ARGC = 4,
    BASE = 10,
    BYTE = 8
};

static unsigned long long res = 0;
static unsigned long long i = 0;

int
sq_sum(unsigned long long x, int mod)
{
    for (; i <= x; i++) {
        res += (i % mod) * (i % mod) % mod;
        res %= mod;
    }
    return (int) res;
}

int
open_safe(char *path, int flag)
{
    int fd = open(path, flag, 0600);
    if (fd == -1) {
        exit(1);
    }
    return fd;
}

int
strtol_safe(char *digit)
{
    errno = 0;
    char *endptr = NULL;
    int a = strtol(digit, &endptr, BASE);
    if (errno || *endptr || endptr == digit) {
        exit(1);
    }
    return a;
}

int
read_safe(int fd, unsigned char *buffer)
{
    ssize_t a = read(fd, buffer, sizeof(*buffer));
    if (a == -1) {
        exit(1);
    }
    return a;
}

int
write_safe(int fd, int *digit)
{
    ssize_t a = write(fd, digit, sizeof(*digit));
    if (a == -1) {
        exit(1);
    }
    return a;
}

int
main(int argc, char **argv)
{
    if (argc < SUPPOSED_ARGC) {
        exit(1);
    }
    int input_file = open_safe(argv[1], O_RDONLY);
    int output_file = open_safe(argv[2], O_WRONLY | O_TRUNC | O_CREAT);
    int mod = strtol_safe(argv[3]);
    unsigned long long cur = 1;
    unsigned char reader = 0;
    while (read_safe(input_file, &reader)) {
        unsigned char mask = 1;
        for (int i = 0; i < BYTE; i++) {
            if (reader & mask) {
                int ans = sq_sum(cur, mod);
                write_safe(output_file, &ans);
            }
            mask <<= 1;
            cur++;
        }
    }
    close(input_file);
    close(output_file);
    return 0;
}

