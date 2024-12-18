#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//made by: CDECL

enum
{
    ARGC_N = 2
};

void
ping_pong(FILE *r, FILE *w, int process_n, int m)
{
    int cur;
    while (1) {
        fscanf(r, "%d", &cur);
        fflush(r);
        if (cur >= m) {
            exit(0);
        }
        printf("%d %d\n", process_n, cur++);
        fflush(stdout);
        fprintf(w, "%d\n", cur);
        fflush(w);
    }
}

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    int m;
    sscanf(argv[1], "%d", &m);
    if (m <= 1) {
        printf("Done\n");
        exit(0);
    }
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    FILE *p1[2], *p2[2];
    p1[0] = fdopen(fd1[0], "r");
    p1[1] = fdopen(fd1[1], "w");
    p2[0] = fdopen(fd2[0], "r");
    p2[1] = fdopen(fd2[1], "w");
    fprintf(p1[1], "1\n");
    fflush(p1[1]);
    pid_t pid;
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        fclose(p1[1]);
        fclose(p2[0]);
        ping_pong(p1[0], p2[1], 1, m);
        exit(1);
    }
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        fclose(p1[0]);
        fclose(p2[1]);
        ping_pong(p2[0], p1[1], 2, m);
        exit(1);
    }
    fclose(p1[0]);
    fclose(p1[1]);
    fclose(p2[0]);
    fclose(p2[1]);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    exit(0);
}

