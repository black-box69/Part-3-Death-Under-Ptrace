#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//made by: CDECL

enum
{
    N = 3
};

int
main(void)
{
    int pid;
    for (int i = 1; i <= N; i++) {
        if ((pid = fork()) == -1) {
            exit(1);
        } else if (!pid) {
            char buf[8];
            int k;
            if (read(0, buf, sizeof(buf)) == -1) {
                exit(1);
            }
            sscanf(buf, "%d", &k);
            printf("%d %d\n", i, k * k);
            exit(0);
        }
    }
    for (int i = 1; i <= N; i++) {
        wait(NULL);
    }
    return 0;
}

