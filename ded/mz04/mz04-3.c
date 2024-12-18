#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//made by: ded

const long long SIZEOF_LONG_LONG = sizeof(SIZEOF_LONG_LONG);

int
main(int argc, char **argv)
{
    if (argc != 2) {
        return 1;
    }

    int file_fd = open(argv[1], O_RDWR, 0777);
    if (file_fd < 0) {
        return 1;
    }

    long long min_elem = 0;
    long long read_elem = 0;
    off_t min_offset = 0;
    off_t current_offset = 0;
    int err_check = 0;

    errno = 0;
    if ((err_check = read(file_fd, &min_elem, SIZEOF_LONG_LONG)) != SIZEOF_LONG_LONG) {
        close(file_fd);
        if (err_check == -1) {
            exit(errno);
        }
        return 0;
    }

    current_offset += SIZEOF_LONG_LONG;
    errno = 0;
    while ((err_check = read(file_fd, &read_elem, SIZEOF_LONG_LONG)) == SIZEOF_LONG_LONG) {
        errno = 0;
        if (err_check == -1) {
            close(file_fd);
            exit(errno);
        }
        if (read_elem < min_elem) {
            min_elem = read_elem;
            min_offset = current_offset;
        }
        current_offset += SIZEOF_LONG_LONG;
    }

    if (min_elem == 0 || min_elem == LLONG_MIN) {
        close(file_fd);
        return 0;
    }

    min_elem = -(unsigned long long) min_elem;
    errno = 0;
    if ((err_check = pwrite(file_fd, &min_elem, SIZEOF_LONG_LONG, min_offset)) != SIZEOF_LONG_LONG) {
        if (err_check == -1) {
            close(file_fd);
            exit(errno);
        }
        return 1;
    }

    close(file_fd);
    return 0;
}
