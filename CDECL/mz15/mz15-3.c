#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//made by: CDECL

#define mdiv(a, b) (a >= 0) ? (a % b) : ((b - (-a % b)) % b)

enum
{
    ARGC_N = 2
};

int semid;
int count;

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    if (sscanf(argv[1], "%d", &count) == -1) {
        exit(1);
    }
    semid = semget(12502, count, 0666 | IPC_CREAT);
    pid_t pid;
    int cur;
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    for (int i = 0; i < count; i++) {
        if ((pid = fork()) == -1) {
            semctl(semid, 0, IPC_RMID, 0);
            for (int j = 0; j <= i; j++) {
                wait(NULL);
            }
            exit(1);
        } else if (!pid) {
            while (semop(semid, (struct sembuf[]){{i, -1, 0}}, 1) >= 0 && scanf("%d", &cur) == 1) {
                printf("%d %d\n", i, cur);
                semop(semid, (struct sembuf[]){{mdiv(cur, count), 1, 0}}, 1);
            }
            semctl(semid, 0, IPC_RMID, 0);
            exit(0);
        }
    }
    semop(semid, (struct sembuf[]){{0, 1, 0}}, 1);
    for (int i = 0; i < count; i++) {
        wait(NULL);
    }
    semctl(semid, 0, IPC_RMID, 0);
    exit(0);
}

