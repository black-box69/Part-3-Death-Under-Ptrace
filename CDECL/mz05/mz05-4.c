#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//made by: CDECL

int
comp(const void *path1, const void *path2)
{
    const char *str1 = *(char **) path1;
    const char *str2 = *(char **) path2;
    struct stat file1;
    struct stat file2;
    if (stat(str1, &file1) == -1) {
        exit(1);
    }
    if (stat(str2, &file2) == -1) {
        exit(1);
    }
    if (file1.st_ino > file2.st_ino) {
        return 1;
    } else if (file1.st_ino < file2.st_ino) {
        return -1;
    } else {
        return strcmp(str2, str1);
    }
}

int
str_comp(const void *s1, const void *s2)
{
    return strcmp(*(char **) s1, *(char **) s2);
}

int
main(int argc, char **argv)
{
    struct stat st;
    int new_argc = 0;
    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], &st) != -1) {
            argv[new_argc] = argv[i];
            new_argc++;
        }
    }
    if (new_argc > 0) {
        qsort(argv, new_argc, sizeof(argv[0]), comp);
        int j = 1;
        char *prev = argv[0];
        if (new_argc > 1) {
            struct stat st1;
            struct stat st2;
            for (int i = 1; i < new_argc; i++) {
                if (stat(prev, &st1) == -1) {
                    exit(1);
                }
                if (stat(argv[i], &st2) == -1) {
                    exit(1);
                }
                if (st1.st_ino != st2.st_ino) {
                    argv[j] = argv[i];
                    j++;
                }
                prev = argv[i];
            }
            qsort(argv, j, sizeof(*argv), str_comp);
        }
        for (int i = 0; i < j; i++) {
            printf("%s\n", argv[i]);
        }
    }
}

