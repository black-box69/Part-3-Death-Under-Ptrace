#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

//made by: CDECL

enum
{
    STDIN_FD = 0,
    STDOUT_FD = 1,
    STDERR_FD = 2,
    ERR_CODE = 42
};

int
main(int argc, char **argv)
{
    pid_t pid;
    if (argc != 5) {
        return ERR_CODE;
    }
    if ((pid = fork()) == -1) {
        return 1;
    }
    if (!pid) {
        int fd0 = open(argv[2], O_RDONLY, 0660);
        int fd1 = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, 0660);
        int fd2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fd0 == -1 || fd1 == -1 || fd2 == -1) {
            _exit(ERR_CODE);
        }

        if (dup2(fd0, STDIN_FD) == -1) {
            _exit(ERR_CODE);
        }
        close(fd0);
        if (dup2(fd1, STDOUT_FD) == -1) {
            _exit(ERR_CODE);
        }
        close(fd1);
        if (dup2(fd2, STDERR_FD) == -1) {
            _exit(ERR_CODE);
        }
        close(fd2);

        execlp(argv[1], argv[1], NULL);
        _exit(ERR_CODE);

    } else {
        int status;
        wait(&status);
        printf("%d\n", status);
        return 0;
    }
}

