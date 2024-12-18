#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

//made by: ded

enum
{
    SIZE = 1024
};

int
main(int argc, char *argv[])
{

    long long sum_files_size = 0;

    for (int i = 1; i < argc; i++) {
        struct stat stbuf;
        if (lstat(argv[i], &stbuf) == -1) {
            continue;
        }
        if (S_ISREG(stbuf.st_mode) && stbuf.st_size % SIZE == 0 && stbuf.st_nlink == 1 && !S_ISLNK(stbuf.st_mode)) {
            sum_files_size += (long long) stbuf.st_size;
        }
    }

    printf("%lld\n", sum_files_size);
    return 0;
}
