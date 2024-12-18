#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

//made by: CDECL

enum
{
    ARGC_N = 4
};

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    key_t key;
    int nproc, maxval, semid, shmid, *sh_val;
    pid_t pid;

    if (sscanf(argv[1], "%d", &nproc) != 1) {
        exit(1);
    }
    if (nproc > 400 || nproc < 1) {
        exit(1);
    }
    if (sscanf(argv[2], "%d", &key) != 1) {
        exit(1);
    }
    if (sscanf(argv[3], "%d", &maxval) != 1) {
        exit(1);
    }
    if (maxval <= 0) {
        exit(1);
    }

    semid = semget(key, nproc, 0666 | IPC_CREAT);
    shmid = shmget(key, 3 * sizeof(*sh_val), 0666 | IPC_CREAT);
    sh_val = shmat(shmid, 0, 0);
    *sh_val = 1;
    *(sh_val + 1) = 0;
    *(sh_val + 2) = 0;
    for (int i = 0; i < nproc; i++) {
        if ((pid = fork()) == -1) {
            exit(1);
        } else if (!pid) {
            while (1) {
                if (semop(semid, (struct sembuf[]){{i, -1, 0}}, 1) >= 0) {
                    if (*sh_val) {
                        printf("%d %d %d\n", i + 1, *(sh_val + 1), *(sh_val + 2));
                        fflush(stdout);
                        *(sh_val + 1) += 1;
                        if (*(sh_val + 1) > maxval) {
                            *sh_val = 0;
                            semctl(semid, 0, IPC_RMID);
                            exit(0);
                        }
                        *(sh_val + 2) = i + 1;
                        long long cur = *(sh_val + 1) % nproc;
                        long long next = (((cur * cur) % nproc) * ((cur * cur) % nproc)) % nproc;
                        fflush(stdout);
                        semop(semid, (struct sembuf[]){{next, 1, 0}}, 1);
                    } else {
                        semctl(semid, 0, IPC_RMID);
                        exit(0);
                    }
                } else {
                    exit(0);
                }
            }
        }
    }
    semop(semid, (struct sembuf[]){{0, 1, 0}}, 1);
    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, 0, IPC_RMID);
    exit(0);
}

