#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

//made by: ded

int
main(int argc, char **argv)
{
    if (argc != 3) {
        exit(1);
    }

    DIR *dir1 = opendir(argv[1]);
    if (!dir1) {
        exit(1);
    }

    DIR *dir2 = opendir(argv[2]);
    if (!dir2) {
        closedir(dir1);
        exit(1);
    }

    long long sum = 0;
    struct dirent *dir1p, *dir2p;
    struct stat st1, st2;

    while ((dir1p = readdir(dir1))) {
        if (!strcmp(dir1p->d_name, ".") || !strcmp(dir1p->d_name, "..")) {
            continue;
        }

        char path1[PATH_MAX];
        snprintf(path1, sizeof(path1), "%s/%s", argv[1], dir1p->d_name);

        if (lstat(path1, &st1) == -1 || !S_ISREG(st1.st_mode)) {
            continue;
        }

        if (access(path1, W_OK) == -1) {
            continue;
        }

        rewinddir(dir2);

        while ((dir2p = readdir(dir2))) {
            if (!strcmp(dir2p->d_name, ".") || !strcmp(dir2p->d_name, "..")) {
                continue;
            }

            if (strcmp(dir1p->d_name, dir2p->d_name) != 0) {
                continue;
            }

            char path2[PATH_MAX];
            snprintf(path2, sizeof(path2), "%s/%s", argv[2], dir2p->d_name);

            if (stat(path2, &st2) == -1) {
                continue;
            }

            if ((S_ISLNK(st2.st_mode) || (st1.st_ino == st2.st_ino && st1.st_dev == st2.st_dev))) {
                sum += st1.st_size;
                break;
            }
        }
    }

    printf("%lld\n", sum);

    closedir(dir1);
    closedir(dir2);
    return 0;
}
