#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

int
main(void)
{
    int pid = -1;
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (pid) {
        wait(NULL);
        printf("1\n");
        exit(0);
    } else {
        if ((pid = fork()) == -1) {
            exit(1);
        } else if (pid) {
            wait(NULL);
            printf("2 ");
            exit(0);
        } else {
            printf("3 ");
            exit(0);
        }
    }
}

