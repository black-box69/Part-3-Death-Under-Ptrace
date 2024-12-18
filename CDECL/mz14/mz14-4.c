#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//made by: CDECL

enum
{
    ARGC_N = 2
};

int m;
FILE *stream[2];
int proc_num = 0;

void
ping_pong(int sig)
{
    int x;
    pid_t pid;
    fscanf(stream[0], "%d%d", &pid, &x);
    fprintf(stream[1], "%d\n%d\n", getpid(), x + 1);
    if (x >= m) {
        fclose(stream[0]);
        fclose(stream[1]);
        kill(pid, SIGUSR1);
        exit(0);
    }
    printf("%d %d\n", proc_num, x);
    kill(pid, SIGUSR1);
}

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    if (sscanf(argv[1], "%d", &m) != 1) {
        exit(1);
    }
    int fd[2];
    int pid1, pid2;
    pipe(fd);
    stream[0] = fdopen(fd[0], "r");
    stream[1] = fdopen(fd[1], "w");
    setbuf(stream[0], NULL);
    setbuf(stream[1], NULL);
    setbuf(stdout, NULL);
    sigaction(SIGUSR1, &(struct sigaction){.sa_handler = ping_pong, .sa_flags = SA_RESTART}, NULL);
    if ((pid1 = fork()) == -1) {
        exit(1);
    } else if (!pid1) {
        proc_num = 1;
        while (1) {
            pause();
        }
    }
    if ((pid2 = fork()) == -1) {
        exit(1);
    } else if (!pid2) {
        proc_num = 2;
        while (1) {
            pause();
        }
    }
    fclose(stream[0]);
    fprintf(stream[1], "%d\n1\n", pid2);
    fclose(stream[1]);
    kill(pid1, SIGUSR1);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    exit(0);
}

