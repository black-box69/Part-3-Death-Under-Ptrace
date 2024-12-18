#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

//made by: ded

int
main(int argc, char **argv)
{
    if (argc != 5) {
        return 1;
    }

    pid_t pid = 0;
    if ((pid = fork()) == 0) {
        int fd_1 = open(argv[2], O_RDONLY);
        if (fd_1 == -1) {
            _exit(42);
        }

        int fd_2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (fd_2 == -1) {
            close(fd_1);
            _exit(42);
        }

        int fd_3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fd_3 == -1) {
            close(fd_1);
            close(fd_2);
            _exit(42);
        }

        if (dup2(fd_1, STDIN_FILENO) == -1) {
            close(fd_1);
            close(fd_2);
            close(fd_3);
            _exit(42);
        }

        if (dup2(fd_2, STDOUT_FILENO) == -1) {
            close(fd_1);
            close(fd_2);
            close(fd_3);
            _exit(42);
        }
        if (dup2(fd_3, STDERR_FILENO) == -1) {
            close(fd_1);
            close(fd_2);
            close(fd_3);
            _exit(42);
        }

        execlp(argv[1], argv[1], NULL);
        _exit(42);
    }
    int status = 0;
    wait(&status);
    printf("%d\n", status);
    return 0;
}
