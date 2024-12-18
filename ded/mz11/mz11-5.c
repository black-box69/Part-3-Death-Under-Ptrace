#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

//made by: ded

int
process_reverse()
{
    int num;
    if (scanf("%d", &num) != 1) {
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        return 1;
    }
    if (pid == 0) {
        int res = process_reverse();
        if (res != 0) {
            exit(1);
        }
        if (printf("%d\n", num) < 0) {
            exit(1);
        }
        if (fflush(stdout) == EOF) {
            exit(1);
        }
        exit(0);
    } else {
        int status;
        if (waitpid(pid, &status, 0) < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            return 1;
        }
        return 0;
    }
}

int
main(int argc, char **argv)
{
    int res = process_reverse();
    if (res != 0) {
        printf("-1\n");
        fflush(stdout);
    }
    return 0;
}
