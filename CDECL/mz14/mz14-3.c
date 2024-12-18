#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//made by: CDECL

void
exe(char *cmd)
{
    pid_t pid;
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        execlp(cmd, cmd, NULL);
        exit(1);
    }
}

void
s_wait(int *cor)
{
    int status;
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        *cor += 1;
    }
}

void
cr(char *file, char **cmd, size_t *size)
{
    FILE *f = fopen(file, "r");
    getline(cmd, size, f);
    (*cmd)[strlen(*cmd) - 1] = '\0';
    fclose(f);
    exe(*cmd);
}

int
main(int argc, char **argv)
{
    int n;
    if (sscanf(argv[1], "%d", &n) != 1) {
        exit(1);
    }
    int cor = 0;
    char *cmd = NULL;
    size_t size;
    for (int i = 2; i < argc && i <= n + 1; i++) {
        cr(argv[i], &cmd, &size);
    }
    for (int i = 0; i < n; i++) {
        s_wait(&cor);
    }
    for (int i = n + 2; i < argc; i++) {
        cr(argv[i], &cmd, &size);
        s_wait(&cor);
    }
    printf("%d\n", cor);
    return 0;
}

