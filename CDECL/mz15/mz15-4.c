#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

//made by: CDECL

enum
{
    ARGC_N = 6
};

key_t key;
int nproc, msqid;
long long v1, v2, v3, maxv;

struct msgm
{
    long type;
    char msgtext[64];
};

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    if (sscanf(argv[1], "%d", &key) != 1) {
        exit(1);
    }
    if (sscanf(argv[2], "%d", &nproc) != 1) {
        exit(1);
    }
    if (sscanf(argv[3], "%lld", &v1) != 1) {
        exit(1);
    }
    if (sscanf(argv[4], "%lld", &v2) != 1) {
        exit(1);
    }
    if (sscanf(argv[5], "%lld", &maxv) != 1) {
        exit(1);
    }
    msqid = msgget(key, 0666 | IPC_CREAT);
    pid_t pid;
    struct msgm m = {0};
    for (int i = 0; i < nproc; i++) {
        if ((pid = fork()) == -1) {
            msgctl(msqid, IPC_RMID, 0);
            for (int j = 0; j <= i; j++) {
                wait(NULL);
            }
            exit(1);
        } else if (!pid) {
            while (msgrcv(msqid, (void *) &m, sizeof(m.msgtext), i + 1, 0) >= 0) {
                sscanf(m.msgtext, "%lld%lld", &v1, &v2);
                v3 = v1 + v2;
                printf("%d %lld\n", i, v3);
                fflush(stdout);
                if (v3 > maxv) {
                    msgctl(msqid, IPC_RMID, 0);
                    exit(0);
                }
                m.type = v3 % nproc + 1;
                memset(m.msgtext, 0, sizeof(m.msgtext));
                snprintf(m.msgtext, 64, "%lld\n%lld\n", v2, v3);
                msgsnd(msqid, (void *) &m, sizeof(m.msgtext), 0);
            }
            msgctl(msqid, IPC_RMID, 0);
            exit(0);
        }
    }
    m.type = 1;
    memset(m.msgtext, 0, sizeof(m.msgtext));
    snprintf(m.msgtext, 64, "%lld\n%lld\n", v1, v2);
    msgsnd(msqid, (void *) &m, sizeof(m.msgtext), 0);
    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
    msgctl(msqid, IPC_RMID, 0);
    exit(0);
}

