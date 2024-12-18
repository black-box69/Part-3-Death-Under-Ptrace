#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//made by: CDECL

void
suicide(pid_t *pids, int n)
{
    for (int i = 0; i < n; i++) {
        kill(pids[i], SIGKILL);
        wait(NULL);
    }
}

void
cl(int from, int to)
{
    if (from != -1 && to != -1) {
        dup2(from, to);
        close(from);
    } else if (from != -1) {
        close(from);
    }
}

void
exe(char *cmd, int in, int out, int fd, pid_t *pids, int n)
{
    pid_t pid;
    if (!(pid = fork())) {
        cl(in, 0);
        cl(out, 1);
        cl(fd, -1);
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid == -1) {
        cl(in, -1);
        cl(out, -1);
        cl(fd, -1);
        suicide(pids, n);
        exit(1);
    } else {
        pids[n] = pid;
        cl(in, -1);
        cl(out, -1);
    }
}

int
main(int argc, char **argv)
{
    if (argc == 1) {
        return 0;
    }
    if (argc == 2) {
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    int fd1[2], fd2[2];
    pid_t pids[argc - 1];
    if (pipe(fd1) == -1) {
        exit(1);
    }
    exe(argv[1], -1, fd1[1], fd1[0], pids, 0);
    close(fd1[1]);
    for (int i = 2; i < argc - 1; i++) {
        if (pipe(fd2) == -1) {
            suicide(pids, i - 1);
            exit(1);
        }
        exe(argv[i], fd1[0], fd2[1], fd2[0], pids, i - 1);
        fd1[0] = fd2[0];
        close(fd2[1]);
    }
    exe(argv[argc - 1], fd1[0], -1, -1, pids, argc - 1);
    for (int i = 0; i < argc; i++) {
        wait(NULL);
    }
    exit(0);
}

