#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//made by: CDECL

enum
{
    ADD = 0,
    MUL = 1,
    INT_BIT = 32
};

volatile sig_atomic_t mode = 0;

void
handler(int sig)
{
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);
    if (sig == SIGINT) {
        mode = 0;
    } else if (sig == SIGQUIT) {
        mode = 1;
    }
}

int
main(int argc, char **argv)
{
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);
    printf("%d\n", getpid());
    fflush(stdout);
    long long res = 0;
    unsigned long long mask = ~(((unsigned long long) -1) << INT_BIT);
    long long cur = 0;
    while (scanf("%lld", &cur) == 1) {
        if (mode == ADD) {
            res += cur;
            res &= mask;
        } else if (mode == MUL) {
            res *= cur;
            res &= mask;
        }
        printf("%d\n", (int) res);
        fflush(stdout);
    }
}

