#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

//made by: CDECL

int
main(int argc, char **argv)
{
    if (argc != 3) {
        exit(1);
    }
    unsigned long long size = 0;
    char path1[PATH_MAX] = {0};
    char path2[PATH_MAX] = {0};
    struct stat st1;
    struct stat st2;
    DIR *d1 = opendir(argv[1]);
    if (d1 == NULL) {
        exit(1);
    }
    DIR *d2 = opendir(argv[2]);
    if (d2 == NULL) {
        closedir(d1);
        exit(1);
    }
    struct dirent *dd1 = NULL;
    struct dirent *dd2 = NULL;
    int start_point = telldir(d2);

    while ((dd1 = readdir(d1))) {
        snprintf(path1, PATH_MAX, "%s/%s", argv[1], dd1->d_name);
        if (lstat(path1, &st1) == -1) {
            continue;
        }
        if (S_ISREG(st1.st_mode) && !access(path1, W_OK)) {
            seekdir(d2, start_point);
            while ((dd2 = readdir(d2))) {
                snprintf(path2, PATH_MAX, "%s/%s", argv[2], dd2->d_name);
                if (stat(path2, &st2) == -1) {
                    continue;
                }
                if (!strcmp(dd1->d_name, dd2->d_name) && st1.st_ino == st2.st_ino) {
                    size += st1.st_size;
                }
            }
        }
    }
    printf("%llu\n", size);
    if (closedir(d1) == -1 || closedir(d2) == -1) {
        exit(1);
    }
    return 0;
}

