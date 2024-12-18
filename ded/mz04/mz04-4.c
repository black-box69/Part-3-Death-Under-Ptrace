#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

//made by: ded

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

int32_t
big_to_little_endian(int32_t val)
{
    uint32_t v = (uint32_t) val;
    uint32_t b0 = (v >> 24) & 0xFF;
    uint32_t b1 = (v >> 16) & 0xFF;
    uint32_t b2 = (v >> 8) & 0xFF;
    uint32_t b3 = v & 0xFF;
    uint32_t res = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
    return (int32_t) res;
}

void
print_tree(int file_fd, int32_t idx)
{
    struct Node elem;

    if (idx < 0) {
        exit(0);
    }

    errno = 0;
    ssize_t read_bytes = pread(file_fd, &elem, sizeof(elem), idx * sizeof(elem));
    if (read_bytes != sizeof(elem)) {
        exit(errno);
    }

    elem.key = big_to_little_endian(elem.key);
    elem.left_idx = big_to_little_endian(elem.left_idx);
    elem.right_idx = big_to_little_endian(elem.right_idx);

    if (elem.right_idx != 0) {
        print_tree(file_fd, elem.right_idx);
    }

    printf("%d\n", elem.key);

    if (elem.left_idx != 0) {
        print_tree(file_fd, elem.left_idx);
    }
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        return 1;
    }

    errno = 0;
    int file_fd = open(argv[1], O_RDONLY);
    if (file_fd == -1) {
        exit(errno);
    }

    print_tree(file_fd, 0);

    errno = 0;
    if (close(file_fd) == -1) {
        exit(errno);
    }

    return 0;
}
