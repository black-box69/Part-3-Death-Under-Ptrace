#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

//made by: ded

enum
{
    PRC_COUNT = 3,
    BUF_SIZE = 8
};

void
fork_proc(int proc)
{
    char buf[BUF_SIZE];
    if (read(0, buf, sizeof(buf)) < sizeof(buf)) {
        _exit(1);
    }

    int num;

    sscanf(buf, "%d", &num);

    if (printf("%d %d\n", proc, num * num) < 0) {
        _exit(1);
    }

    if (fflush(stdout) == EOF) {
        _exit(1);
    }

    _exit(0);
}

int
main(int argc, char **argv)
{
    pid_t pid[PRC_COUNT];
    for (int i = 0; i < PRC_COUNT; i++) {
        pid[i] = fork();
        if (pid[i] < 0) {
            exit(1);
        }
        if (pid[i] == 0) {
            fork_proc(i + 1);
            _exit(0);
        }
    }
    for (int i = 0; i < PRC_COUNT; i++) {
        if (wait(NULL) < 0) {
            exit(1);
        }
    }
    return 0;
}
