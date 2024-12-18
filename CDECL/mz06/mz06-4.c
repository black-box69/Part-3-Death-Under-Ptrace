#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

//made by: CDECL

const char PR[] = "..";
const char CR[] = ".";

enum
{
    ARGC_N = 3,
    MAX_DEPTH = 4,
    BASE = 10
};

int
strtol_s(char *buf)
{
    char *eptr = NULL;
    errno = 0;
    int res = strtol(buf, &eptr, BASE);
    if (errno || *eptr || eptr == buf) {
        exit(1);
    }
    return res;
}

void
rec_dir(const char *path, int max_size, int depth, const int abs_len)
{
    if (depth > MAX_DEPTH) {
        return;
    }
    struct stat st;
    struct dirent *dd;
    DIR *d = opendir(path);
    if (d == NULL) {
        exit(1);
    }
    while ((dd = readdir(d))) {
        if (strcmp(dd->d_name, PR) != 0 && strcmp(dd->d_name, CR) != 0) {
            char fpath[PATH_MAX] = {0};
            snprintf(fpath, PATH_MAX, "%s/%s", path, dd->d_name);
            if (lstat(fpath, &st) != -1 && !access(fpath, R_OK)) {
                if (S_ISDIR(st.st_mode)) {
                    rec_dir(fpath, max_size, depth + 1, abs_len);
                } else if (st.st_size <= max_size && S_ISREG(st.st_mode)) {
                    const char *rpath = fpath + abs_len;
                    printf("%s\n", rpath);
                }
            }
        }
    }
    closedir(d);
}

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    int Z = strtol_s(argv[2]);
    rec_dir(argv[1], Z, 1, strlen(argv[1]) + 1);
}

