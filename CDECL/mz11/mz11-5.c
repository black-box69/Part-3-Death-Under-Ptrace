#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//made by: CDECL

int
main(void)
{
    int pid;
    int k;
    for (int i = 0; scanf("%d", &k) == 1; i++) {
        if ((pid = fork()) == -1) {
            if (i == 0) {
                printf("-1\n");
                return 0;
            } else {
                exit(1);
            }
        } else if (pid) {
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("%d\n", k);
                exit(0);
            } else {
                if (i == 0) {
                    printf("-1\n");
                    return 0;
                } else {
                    exit(1);
                }
            }
        }
    }
}

