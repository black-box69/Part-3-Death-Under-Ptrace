#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//made by: ded

int
main(int argc, char **argv)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        exit(1);
    }

    if (pid1 == 0) {
        pid_t pid2 = fork();
        if (pid2 == -1) {
            _exit(1);
        }
        if (pid2 == 0) {
            if (close(pipefd[1]) == -1) {
                _exit(1);
            }
            int num = 0;
            long long sum = 0;
            while (read(pipefd[0], &num, sizeof(num))) {
                sum += num;
            }
            if (close(pipefd[0]) == -1) {
                _exit(1);
            }
            printf("%lld\n", sum);
            fflush(stdout);
            _exit(0);
        } else {
            if (close(pipefd[0]) == -1) {
                exit(1);
            }
            if (close(pipefd[1]) == -1) {
                exit(1);
            }
            if (wait(NULL) == -1) {
                exit(1);
            }
            exit(0);
        }
    } else {
        if (close(pipefd[0]) == -1) {
            exit(1);
        }
        int num = 0;
        while (scanf("%d", &num) == 1) {
            if (write(pipefd[1], &num, sizeof(num)) != sizeof(num)) {
                exit(1);
            }
        }
        if (close(pipefd[1]) == -1) {
            exit(1);
        }
        if (wait(NULL) == -1) {
            exit(1);
        }
    }
    return 0;
}
