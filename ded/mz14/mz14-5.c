#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//made by: ded

volatile sig_atomic_t sigint_cnt = 0;
volatile sig_atomic_t simple_n = 0;

void
SIGINT_hndlr(int sig)
{
    signal(SIGINT, SIGINT_hndlr);
    sigint_cnt += 1;
    if (sigint_cnt >= 4) {
        exit(0);
    }
    printf("%d\n", simple_n);
    fflush(stdout);
}

void
SIGTERM_hndlr(int sig)
{
    signal(SIGTERM, SIGTERM_hndlr);
    exit(0);
}

int
is_simple(int num)
{
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char **argv)
{
    signal(SIGINT, SIGINT_hndlr);
    signal(SIGTERM, SIGTERM_hndlr);
    int low, high;
    scanf("%d%d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    for (int i = low; i < high; i++) {
        if (is_simple(i)) {
            simple_n = i;
        }
    }

    printf("%d\n", -1);
    fflush(stdout);
    return 0;
}
