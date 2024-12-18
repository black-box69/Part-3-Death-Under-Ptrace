#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

//made by: ded

volatile sig_atomic_t counter = 0;

enum
{
    PROC_NUM = 5
};

void
hndlr(int signal)
{
    if (counter < 5) {
        printf("%d\n", counter++);
        fflush(stdout);
        return;
    } else {
        exit(0);
    }
}

int
main(int argc, char **argv)
{
    if (sigaction(SIGHUP, &(struct sigaction){.sa_handler = hndlr, .sa_flags = SA_RESTART}, NULL) < 0) {
        exit(1);
    }

    printf("%d\n", getpid());
    fflush(stdout);

    while (1) {
        pause();
    }
}
