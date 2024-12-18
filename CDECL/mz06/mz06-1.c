#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <fcntl.h>

//made by: CDECL

const char FORM[] = ".exe";

enum
{
    NORMAL_ARGC = 2,
    LEN_FORM = sizeof(FORM) - 1
};

int
exe_check(char *name)
{
    if (strlen(name) < LEN_FORM) {
        return 0;
    }
    if (strcmp(&name[strlen(name) - LEN_FORM], FORM) == 0) {
        return 1;
    }
    return 0;
}

int
main(int argc, char **argv)
{
    if (argc < NORMAL_ARGC) {
        exit(1);
    }
    DIR *d = opendir(argv[1]);
    if (d == NULL) {
        exit(1);
    }
    int ans = 0;
    struct stat st;
    struct dirent *cur;
    while ((cur = readdir(d))) {
        char path[PATH_MAX] = {0};
        snprintf(path, PATH_MAX, "%s/%s", argv[1], cur->d_name);
        if (path[PATH_MAX - 1] != '\0') {
            exit(1);
        }
        stat(path, &st);
        if (S_ISREG(st.st_mode) && !access(path, X_OK) && exe_check(path)) {
            ans++;
        }
    }
    printf("%d\n", ans);
    closedir(d);
}

