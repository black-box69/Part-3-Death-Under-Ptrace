#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//made by: CDECL

enum
{
    ARGC_NORMAL = 4
};

int
exe(char *cmd)
{
    int pid;
    if ((pid = fork()) == -1) {
        _exit(1);
    } else if (!pid) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return 1;
        } else {
            return 0;
        }
    }
}

int
main(int argc, char **argv)
{
    if (argc != ARGC_NORMAL) {
        _exit(1);
    }

    if ((exe(argv[1]) || exe(argv[2])) && exe(argv[3])) {
        _exit(0);
    } else {
        _exit(1);
    }
}

