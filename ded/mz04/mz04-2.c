#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

//made by: ded

enum
{
    BYTES_COUNT = 8,
    STEP = -8
};

int
main(int argc, char **argv)
{
    int open_fd = open(argv[1], O_RDWR);
    int N = atoi(argv[2]);

    double read_num_1 = 0;
    double read_num_2 = 0;

    for (int i = 1; i < N; ++i) {
        int read_count_1 = read(open_fd, &read_num_1, sizeof(double));

        if (read_count_1 > 0) {

            int read_count_2 = read(open_fd, &read_num_2, sizeof(double));
            if (read_count_2 > 0) {
                lseek(open_fd, STEP, SEEK_CUR);
                double res_num = read_num_2 - read_num_1;
                write(open_fd, &res_num, BYTES_COUNT);
                lseek(open_fd, STEP, SEEK_CUR);
            } else {
                break;
            }
        } else {
            break;
        }
    }

    close(open_fd);
    return 0;
}
