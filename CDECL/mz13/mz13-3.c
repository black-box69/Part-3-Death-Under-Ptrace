#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

//made by: CDECL

enum
{
    ARGC_N = 6
};

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

int
exe(char *cmd, int in, int out)
{
    pid_t pid;
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        cl(in, 0);
        cl(out, 1);
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else {
        int status;
        wait(&status);
        cl(in, -1);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return 1;
        }
    }
    return 0;
}

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    int fd[2];
    pipe(fd);
    int f1, f2;
    pid_t pid;
    if ((f1 = open(argv[4], O_RDONLY)) == -1) {
        exit(1);
    }
    if ((f2 = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0666)) == -1) {
        exit(1);
    }
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        close(fd[0]);
        close(f2);
        if (exe(argv[1], f1, fd[1])) {
            close(f1);
            exe(argv[2], -1, fd[1]);
            close(fd[1]);
        }
        close(fd[1]);
    } else {
        close(fd[1]);
        close(f1);
        exe(argv[3], fd[0], f2);
        wait(NULL);
        close(fd[1]);
        close(fd[0]);
        close(f2);
    }
    exit(0);
}

