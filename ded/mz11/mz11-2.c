#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

//made by: ded

int
main(int argc, char **argv)
{
    pid_t pid_1 = 0;
    pid_1 = fork();
    if (pid_1 < 0) {
        _exit(1);
    }
    if (pid_1 == 0) {
        pid_t pid_2 = 0;
        pid_2 = fork();
        if (pid_2 < 0) {
            _exit(1);
        }
        if (pid_2 == 0) {
            if (printf("3 ") < 0) {
                _exit(1);
            }
            if (fflush(stdout) == EOF) {
                _exit(1);
            }
            _exit(0);
        }
        if (wait(NULL) < 0) {
            _exit(1);
        }
        if (printf("2 ") < 0) {
            _exit(1);
        }
        if (fflush(stdout) == EOF) {
            _exit(1);
        }
        _exit(0);
    }
    if (wait(NULL) < 0) {
        exit(1);
    }
    if (printf("1\n") < 0) {
        exit(1);
    }
    if (fflush(stdout) == EOF) {
        exit(1);
    }
    return 0;
}
