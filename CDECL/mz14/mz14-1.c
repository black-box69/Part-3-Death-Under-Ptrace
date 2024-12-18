#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//made by: CDECL

volatile sig_atomic_t count = 0;

enum
{
    MAX_VAL = 5
};

void
handler(int sig)
{
    if (count >= MAX_VAL) {
        exit(0);
    }
    printf("%d\n", count++);
    fflush(stdout);
}

int
main(int argc, char **argv)
{
    if (sigaction(SIGHUP, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL) == -1) {
        exit(1);
    }
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
}

