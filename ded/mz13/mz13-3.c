#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

//made by: ded

enum
{
    STDIN_FD = 0,
    STDOUT_FD = 1
};

int
main(int argc, char **argv)
{
    if (argc < 6) {
        return 1;
    }
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        _exit(1);
    }
    pid_t pid1 = 0;
    if ((pid1 = fork()) == 0) {
        close(pipe_fd[0]);
        if (dup2(pipe_fd[1], STDOUT_FD) < 0) {
            _exit(1);
        }
        close(pipe_fd[1]);

        pid_t pid2 = 0;
        if ((pid2 = fork()) == 0) {
            int file1_fd = open(argv[4], O_RDONLY);
            if (file1_fd < 0) {
                _exit(1);
            }
            if (dup2(file1_fd, STDIN_FD) < 0) {
                close(file1_fd);
                _exit(1);
            }
            close(file1_fd);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }

        if (pid2 < 0) {
            _exit(1);
        }

        int status = 0;
        wait(&status);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            pid_t pid3 = 0;
            if ((pid3 = fork()) == 0) {
                execlp(argv[2], argv[2], NULL);
                _exit(1);
            }
            if (pid3 < 0) {
                _exit(1);
            }
        } else {
            _exit(1);
        }

        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            _exit(0);
        }
        _exit(1);
    }
    if (pid1 < 0) {
        return 1;
    }
    close(pipe_fd[1]);

    pid_t pid4 = 0;
    if ((pid4 = fork()) == 0) {
        if (dup2(pipe_fd[0], STDIN_FD) < 0) {
            close(pipe_fd[0]);
            _exit(1);
        }
        close(pipe_fd[0]);
        int file2_fd = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0600);
        if (file2_fd < 0) {
            _exit(1);
        }
        if (dup2(file2_fd, STDOUT_FD) < 0) {
            close(file2_fd);
            _exit(1);
        }
        close(file2_fd);
        execlp(argv[3], argv[3], NULL);
        _exit(1);
    } else if (pid4 < 0) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        _exit(1);
    }
    close(pipe_fd[0]);

    int pid1_status, pid4_status;
    if (waitpid(pid1, &pid1_status, 0) == -1) {
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid4, &pid4_status, 0) == -1) {
        exit(EXIT_FAILURE);
    }
    return 0;
}
