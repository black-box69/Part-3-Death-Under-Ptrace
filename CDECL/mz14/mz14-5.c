#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>

//made by: CDECL

int is_prime(int num) {
    for (int i = 2; i * i <= num; i++) {
        if ((num % i) == 0) {
            return 0;
        }
    }
    return 1;
}

enum
{
    MAX = 3
};

volatile sig_atomic_t cur = 0;
volatile sig_atomic_t c = 0;

void hndlr(int s) {
    if (s == SIGINT) {
        if (c >= MAX) {
            exit(0);
        }
        printf("%d\n", cur);
        fflush(stdout);
    } else if (s == SIGTERM) {
        exit(0);
    }
}


int main(void) {
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = hndlr, .sa_flags = SA_RESTART}, 0);
    sigaction(SIGTERM, &(struct sigaction) {.sa_handler = hndlr, .sa_flags = SA_RESTART}, 0);
    int low, high;
    scanf("%d%d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    low = (low < 2)?2:low;
    for (int i = low; i < high; i++) {
        if (is_prime(i)) {cur = i;}
    }
    printf("-1\n");
    fflush(stdout);
    exit(0);
}

