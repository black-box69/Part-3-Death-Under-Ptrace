#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

//made by: CDECL

enum
{
    FM = 1,
    FY = 1900
};

int
main(void)
{
    pid_t pid;
    struct tm tt;
    int fd[2];
    pipe(fd);
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        if ((pid = fork()) == -1) {
            exit(1);
        } else if (!pid) {
            if ((pid = fork()) == -1) {
                exit(1);
            } else if (!pid) {
                close(fd[0]);
                time_t t = time(NULL);
                localtime_r(&t, &tt);
                tt.tm_isdst = -1;
                write(fd[1], &tt.tm_mday, sizeof(tt.tm_mday));
                write(fd[1], &tt.tm_mon, sizeof(tt.tm_mon));
                write(fd[1], &tt.tm_year, sizeof(tt.tm_year));
                close(fd[1]);
                exit(0);
            } else {
                close(fd[1]);
                wait(NULL);
                read(fd[0], &tt.tm_mday, sizeof(tt.tm_mday));
                printf("D:%.2d\n", tt.tm_mday);
                close(fd[0]);
                exit(0);
            }
        } else {
            close(fd[1]);
            wait(NULL);
            read(fd[0], &tt.tm_mon, sizeof(tt.tm_mon));
            printf("M:%.2d\n", tt.tm_mon + FM);
            close(fd[0]);
            exit(0);
        }
    } else {
        close(fd[1]);
        wait(NULL);
        read(fd[0], &tt.tm_year, sizeof(tt.tm_year));
        printf("Y:%d\n", tt.tm_year + FY);
        close(fd[0]);
        exit(0);
    }
}

