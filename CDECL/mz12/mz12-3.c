#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

//made by: CDECL

enum
{
    PARALLEL = 'p',
    STEP = 's'
};

void
avoid_sync_err(int n, int *ans)
{
    int status;
    for (int i = 0; i < n; i++) {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            *ans = *ans + 1;
        }
    }
}

int
main(int argc, char **argv)
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    int counter = 0;
    int working = 0;
    pid_t pid = 0;
    int status;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == PARALLEL) {
            working++;
            if ((pid = fork()) == -1) {
                avoid_sync_err(working, &counter);
                _exit(1);
            } else if (!pid) {
                execlp(argv[i] + 1, argv[i] + 1, NULL);
                _exit(1);
            }
        } else if (argv[i][0] == STEP) {
            avoid_sync_err(working, &counter);
            working = 0;
            if ((pid = fork()) == 0) {
                execlp(argv[i] + 1, argv[i] + 1, NULL);
                _exit(1);
            } else if (pid == -1) {
                avoid_sync_err(working, &counter);
                _exit(1);
            }
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                counter++;
            }
        } else {
            _exit(1);
        }
    }
    avoid_sync_err(working, &counter);
    printf("%d\n", counter);
    _exit(0);
}

