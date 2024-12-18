#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//made by: ded

int
comparator(const void *a, const void *b)
{
    const char *str1 = *(const char **) a;
    const char *str2 = *(const char **) b;
    return strcmp(str1, str2);
}

int
main(int argc, char **argv)
{
    char *id_mas = malloc(argc * sizeof(char));
    memset(id_mas, 0, argc);

    char **mas2sort = malloc(argc * sizeof(id_mas));
    int uniq_cnt = 0;

    for (int i = 1; i < argc; i++) {
        if (id_mas[i] == 1) {
            continue;
        }
        struct stat main_stbuf;
        char *buf_id = argv[i];
        errno = 0;
        if (stat(buf_id, &main_stbuf) == -1 || errno) {
            continue;
        }
        id_mas[i] = 1;
        for (int j = i + 1; j < argc; j++) {
            struct stat runner_stbuf;
            errno = 0;
            if (stat(argv[j], &runner_stbuf) == -1 || errno) {
                continue;
            }
            if (runner_stbuf.st_dev != main_stbuf.st_dev) {
                continue;
            }
            if (runner_stbuf.st_ino != main_stbuf.st_ino) {
                continue;
            }
            id_mas[j] = 1;
            if (strcmp(argv[j], buf_id) > 0) {
                buf_id = argv[j];
            }
        }
        mas2sort[uniq_cnt] = buf_id;
        uniq_cnt++;
    }
    qsort(mas2sort, uniq_cnt, sizeof(*mas2sort), comparator);
    for (int i = 0; i < uniq_cnt; i++) {
        printf("%s\n", mas2sort[i]);
    }
    free(id_mas);
    free(mas2sort);
    return 0;
}
