#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//made by: CDECL

int
main(void)
{
    pid_t pid;
    int cur;
    int fd[2];
    pipe(fd);
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        if ((pid = fork()) == -1) {
            exit(1);
        } else if (!pid) {
            close(fd[1]);
            long long res = 0;
            int check = 0;
            while ((check = read(fd[0], &cur, sizeof(cur))) > 0) {
                if (check == 1 && (char) cur == EOF) {
                    break;
                }
                res += cur;
            }
            printf("%lld\n", res);
            fflush(stdout);
            exit(0);
        } else {
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            exit(0);
        }
    } else {
        close(fd[0]);
        while (scanf("%d", &cur) == 1) {
            write(fd[1], &cur, sizeof(cur));
        }
        char end = EOF;
        write(fd[1], &end, sizeof(end));
        close(fd[1]);
        wait(NULL);
        exit(0);
    }
}

