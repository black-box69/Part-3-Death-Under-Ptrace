#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

//made by: CDECL

pid_t pid, pid2;
int fd;
unsigned char buf = 0, cur;
int i = 0;

void
recv_b(int s, siginfo_t *info, void *context)
{
    fflush(stdout);
    if (s == SIGIO) {
        exit(0);
    }
    if (info != NULL) {
        pid = info->si_pid;
    }
    if (s == SIGUSR2) {
        buf |= (1u << i);
    }
    i++;
    fflush(stdout);
    if (i == 8) {
        i = 0;
        printf("%c", (char) buf);
        fflush(stdout);
        buf = 0;
    }
    kill(pid, SIGALRM);
}

void
send_b(int s)
{
    fflush(stdout);
    if (i == 8) {
        if (read(fd, &buf, sizeof(buf)) != 1) {
            fflush(stdout);
            close(fd);
            kill(pid, SIGIO);
            exit(0);
        }
        i = 0;
        fflush(stdout);
    }
    i++;
    cur = buf & 1u;
    buf >>= 1;
    if (cur == 0) {
        kill(pid, SIGUSR1);
    } else if (cur == 1) {
        kill(pid, SIGUSR2);
    }
}

int
main(int argc, char **argv)
{
    if ((pid = fork()) == 0) {
        sigaction(SIGUSR1, &(struct sigaction){.sa_sigaction = recv_b, .sa_flags = SA_SIGINFO}, 0);
        sigaction(SIGUSR2, &(struct sigaction){.sa_sigaction = recv_b, .sa_flags = SA_SIGINFO}, 0);
        sigaction(SIGIO, &(struct sigaction){.sa_sigaction = recv_b, .sa_flags = SA_SIGINFO}, 0);
        while (1) {
            pause();
        }
        exit(0);
    }
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        kill(pid, SIGIO);
        wait(NULL);
        exit(1);
    };
    if ((pid2 = fork()) == 0) {
        sigaction(SIGALRM, &(struct sigaction){.sa_handler = send_b, .sa_flags = SA_RESTART}, 0);
        if (read(fd, &buf, sizeof(buf)) != 1) {
            kill(pid, SIGIO);
            exit(0);
        }
        fflush(stdout);
        kill(getpid(), SIGALRM);
        while (1) {
            pause();
        }
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    exit(0);
}

