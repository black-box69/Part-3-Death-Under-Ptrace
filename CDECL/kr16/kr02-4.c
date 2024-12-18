#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

//made by: CDECL

int f;
unsigned bias;
int fd[2];

void handler(int s) {
    unsigned addr;
    read(fd[0], &addr, sizeof(addr));
    int ans;
    unsigned char val;
    addr >>= bias;
    if (lseek(f, addr, SEEK_SET) == -1 || read(f, &val, sizeof(val)) == -1) {
        ans = -1;
        write(fd[1], &ans, sizeof(ans));
    } else {
        ans = (int) val;
        write(fd[1], &ans, sizeof(ans));
    }
}

void genocide(pid_t *pids, int n) {
    for (int i = 0; i < n; i++) {
        kill(pids[i], SIGKILL);
        wait(NULL);
    }
}

int find(int a) {
    int b = 1;
    int i = 0;
    while (b < a) {
        b *= 2;
        i++;
    }
    return i;
}

int main(int argc, char **argv) {
    sigaction(SIGUSR1, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, 0);
    pipe(fd);
    pid_t pids[argc - 1];
    bias = find(argc - 1);
    long long unsigned mask = ((1u << find(argc - 1)) - 1);
    if (argc < 2) exit(1);
    for (int i = 0; i < argc - 1; i++) {
        f = open(argv[i + 1], O_RDONLY);
        if (f < 0) {
            genocide(pids, i);
            exit(1);
        }
        if ((pids[i] = fork()) == -1) {
            genocide(pids, i);
            exit(1);
        } else if (!pids[i]) {
            while (1) {
                pause();
            }
        }
        close(f);
    }
    unsigned addr = 0;
    int val = 0;
    while (scanf("%u", &addr) == 1) {
        if ((addr & mask) < argc - 1) {
            write(fd[1], &addr, sizeof(addr));
            kill(pids[addr & mask], SIGUSR1);
            read(fd[0], &val, sizeof(val));
            printf("%d\n", val);
        } else {
            printf("-1\n");
        }
    }
    genocide(pids, argc - 1);
    exit(0);
}

