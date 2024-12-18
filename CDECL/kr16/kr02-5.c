#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <limits.h>

//made by: CDECL

long long BR = ((long long) INT_MIN) - 1;

int msqid;

struct msgp {
    long type;
    char buf[40];
};

void genocide(pid_t pids[], int n) {
    for (int i = 1; i < n; i++) {
        kill(pids[i], SIGKILL);
        wait(NULL);
    }
}

int min(long long *vals, int n) {
    long long d = INT_MAX;
    int i = 0;
    int flg = 0;
    for (int j = 0; j < n; j++) {
        if (d >= vals[j] && vals[j] >= INT_MIN) {
            d = vals[j];
            i = j;
            flg = 1;
        }
    }
    if (!flg) {
        return -1;
    }
    return i;
}

int main(int argc, char **argv) {
    msqid = msgget(1234, 0666 | IPC_CREAT);
    pid_t pids[argc];
    pids[0] = getpid();
    FILE *f;
    struct msgp m = {0};
    for (int i = 1; i < argc; i++) {
        if ((f = fopen(argv[i], "r")) == NULL || (pids[i] = fork()) == -1) {
            msgctl(msqid, IPC_RMID, 0);
            genocide(pids, i);
            exit(1);
        } else if (!pids[i]) {
            m.type = i;
            long long digit;
            while (1) {
                msgrcv(msqid, &m, sizeof(m.buf), argc + i - 1, 0);
                m.type = i;
                if (fscanf(f, "%lld", &digit) != 1) {
                    fclose(f);
                    sprintf(m.buf, "%lld", BR);
                    msgsnd(msqid, &m, sizeof(m.buf), 0);
                    exit(0);
                }
                sprintf(m.buf, "%lld", digit);
                msgsnd(msqid, &m, sizeof(m.buf), 0);
            }
        }
        fclose(f);
    }
    long long vals[argc - 1];
    for (int i = 0; i < argc - 1; i++) {
        m.type = argc + i;
        msgsnd(msqid, &m, sizeof(m.buf), 0);
        msgrcv(msqid, &m, sizeof(m.buf), i + 1, 0);
        sscanf(m.buf, "%lld", &vals[i]);
    }
    int idx = 0;
    while ((idx = min(vals, argc - 1)) != -1) {
        printf("%lld\n", vals[idx]);
        m.type = argc + idx;
        msgsnd(msqid, &m, sizeof(m.buf), 0);
        msgrcv(msqid, &m, sizeof(m.buf), idx + 1, 0);
        sscanf(m.buf, "%lld", &vals[idx]);
    }
    msgctl(msqid, IPC_RMID, 0);
    genocide(pids, argc);
    exit(0);
}

