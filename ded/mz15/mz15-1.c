#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

//made by: ded

void
worker(int semid, int shmid, int *pmem, int self, int nproc, int maxval)
{
    while (1) {
        struct sembuf op = {self - 1, -1, 0};
        if (semop(semid, &op, 1) < 0) {
            break;
        }

        if (*pmem > maxval) {
            for (int i = 0; i < nproc; i++) {
                struct sembuf release = {i, 1, 0};
                semop(semid, &release, 1);
            }
            break;
        }

        printf("%d %d %d\n", self, *pmem, pmem[1]);
        fflush(stdout);
        (*pmem)++;
        int it = pmem[0] % nproc;
        int num = ((((it * it) % nproc) * it) % nproc * it) % nproc;
        pmem[1] = self;

        struct sembuf release = {num, 1, 0};
        if (semop(semid, &release, 1) < 0) {
            break;
        }
    }
    exit(0);
}

int
main(int argc, char **argv)
{
    if (argc < 4) {
        exit(1);
    }

    int nproc = strtol(argv[1], NULL, 10);
    int key = strtol(argv[2], NULL, 10);
    int maxval = strtol(argv[3], NULL, 10);
    int *pmem = NULL;
    size_t shm_size = 2 * sizeof(*pmem);
    int semid = semget(key, nproc, 0600 | IPC_CREAT);
    if (semid < 0) {
        exit(1);
    }
    int shmid = shmget(key, shm_size, 0600 | IPC_CREAT);
    if (shmid < 0) {
        semctl(semid, 0, IPC_RMID);
        exit(1);
    }
    pmem = shmat(shmid, NULL, 0);
    if (pmem == (void *) -1) {
        semctl(semid, 0, IPC_RMID);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }
    pmem[0] = 0;
    pmem[1] = 0;

    for (int i = 1; i <= nproc; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            for (int j = 1; j < i; j++) {
                wait(NULL);
            }
            semctl(semid, 0, IPC_RMID);
            shmctl(shmid, IPC_RMID, NULL);
            exit(1);
        }
        if (pid == 0) {
            worker(semid, shmid, pmem, i, nproc, maxval);
        }
    }
    struct sembuf start = {0, 1, 0};
    if (semop(semid, &start, 1) < 0) {
        perror("semop start");
    }

    while (1) {
        if (wait(NULL) < 0) {
            break;
        }
    }

    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    shmdt(pmem);
    return 0;
}
