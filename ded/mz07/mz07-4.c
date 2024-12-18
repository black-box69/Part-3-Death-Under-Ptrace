#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//made by: ded

enum
{
    BYTE_SIZE = 8,
    POSITIVE_VALUE = 1,
    NEGATIVE_VALUE = 0,
    BYTE_CNT = 1
};

int
my_abs(int num)
{
    return (num > 0) ? num : -num;
}

void
swap(char *arg, int offset, int val)
{
    int byte_mask = 1 << offset;
    if (val == POSITIVE_VALUE) {
        *arg |= byte_mask;
    } else {
        *arg &= ~byte_mask;
    }
}

int
main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDWR);

    long long file_size = lseek(fd, 0, SEEK_END) * BYTE_SIZE;
    lseek(fd, 0, SEEK_SET);

    int cur_num = 0, pos = 0, offset = 0, bit = 0;
    char tmp = 0;
    while (scanf("%d", &cur_num) > 0) {
        if (cur_num == 0) {
            continue;
        }
        if (cur_num > 0) {
            if (cur_num > file_size) {
                continue;
            } else {
                pos = my_abs(cur_num - 1) / BYTE_SIZE;
                offset = my_abs(cur_num - 1) % BYTE_SIZE;
                bit = POSITIVE_VALUE;
            }
        } else {
            if (cur_num < -file_size) {
                continue;
            } else {
                pos = my_abs(cur_num + 1) / BYTE_SIZE;
                offset = my_abs(cur_num + 1) % BYTE_SIZE;
                bit = NEGATIVE_VALUE;
            }
        }
        if (pread(fd, &tmp, BYTE_CNT, pos) < BYTE_CNT) {
            close(fd);
            exit(1);
        }
        swap(&tmp, offset, bit);
        if (pwrite(fd, &tmp, BYTE_CNT, pos) < BYTE_CNT) {
            close(fd);
            exit(1);
        }
    }
    close(fd);
}
