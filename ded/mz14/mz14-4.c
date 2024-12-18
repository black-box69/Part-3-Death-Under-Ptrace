#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//made by: ded

FILE *fds[2];
int id, head;

void
son(int sig)
{
    signal(SIGUSR1, son);

    pid_t pid = 0;
    int num = 0;

    fscanf(fds[0], "%d%d", &pid, &num);
    fprintf(fds[1], "%d\n%d\n", getpid(), num + 1);

    if (num >= head) {
        fclose(fds[0]);
        fclose(fds[1]);
        kill(pid, SIGUSR1);
        exit(0);
    }

    printf("%d %d\n", id, num);
    kill(pid, SIGUSR1);
}

int
main(int argc, char **argv)
{
    signal(SIGUSR1, son);
    int pipe_fd[2];
    pipe(pipe_fd);

    fds[0] = fdopen(pipe_fd[0], "r");
    fds[1] = fdopen(pipe_fd[1], "w");
    setbuf(fds[0], NULL);
    setbuf(fds[1], NULL);
    setbuf(stdout, NULL);

    sscanf(argv[1], "%d", &head);

    pid_t pid1 = fork();
    if (pid1 < 0) {
        _exit(1);
    }
    if (pid1 == 0) {
        id = 1;
        while (1) {
            pause();
        }
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        _exit(1);
    }
    if (pid2 == 0) {
        id = 2;
        while (1) {
            pause();
        }
    }

    fprintf(fds[1], "%d\n%d\n", pid2, 1);
    fclose(fds[0]);
    fclose(fds[1]);

    kill(pid1, SIGUSR1);

    wait(NULL);
    wait(NULL);

    printf("Done\n");
    return 0;
}
