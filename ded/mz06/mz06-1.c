#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

//made by: ded

int
main(int argc, char **argv)
{
    DIR *dir = opendir(argv[1]);
    if (!dir) {
        exit(1);
    }
    struct dirent *dd;
    long long cntr = 0;

    uid_t uid = getuid();
    gid_t gid = getgid();

    while ((dd = readdir(dir))) {
        if (!strcmp(dd->d_name, ".") || !strcmp(dd->d_name, "..")) {
            continue;
        }
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name);
        size_t name_len = strlen(dd->d_name);
        struct stat stb;
        if (stat(path, &stb) < 0) {
            continue;
        }
        if (!S_ISREG(stb.st_mode)) {
            continue;
        }

        if (name_len < 4) {
            continue;
        }
        if (strcmp((dd->d_name + (name_len - 4)), ".exe") != 0) {
            continue;
        }

        mode_t perm = stb.st_mode;
        if (stb.st_uid == uid) {
            if (!(perm & S_IXUSR)) {
                continue;
            }
        } else if (stb.st_gid == gid) {
            if (!(perm & S_IXGRP)) {
                continue;
            }
        } else {
            if (!(perm & S_IXOTH)) {
                continue;
            }
        }
        ++cntr;
    }
    printf("%lld\n", cntr);
    closedir(dir);
    return 0;
}
