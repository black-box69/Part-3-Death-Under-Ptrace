#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

//made by: ded

int
main(int argc, char **argv)
{
    setbuf(stdin, NULL);
    int limit = 0;
    if (scanf("%d", &limit) < 1 || limit < 1) {
        return 0;
    }
    int i = 1;
    while (i < limit) {
        printf("%d ", i);
        fflush(stdout);
        int pid = 0;
        pid = fork();
        if (pid < 0) {
            exit(1);
        }
        if (pid != 0) {
            wait(NULL);
            _exit(0);
        } else {
            i++;
        }
    }
    printf("%d\n", i);
    fflush(stdout);
    return 0;
}
