#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//made by: CDECL

int
main(void)
{
    int n;
    scanf("%d", &n);
    if (n <= 0) {
        exit(1);
    }
    int pid;

    if (n > 1) {
        printf("1 ");
        fflush(stdout);
    } else if (n == 1) {
        printf("1");
    }

    for (int i = 2; i <= n; i++) {
        fflush(stdout);
        if ((pid = fork()) == -1) {
            exit(1);
        } else if (!pid) {
            printf("%d", i);
            if (i != n) {
                printf(" ");
            }
            fflush(stdout);
        } else {
            wait(NULL);
            exit(0);
        }
    }
    printf("\n");
    return 0;
}

