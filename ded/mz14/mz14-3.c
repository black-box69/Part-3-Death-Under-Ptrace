#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//made by: ded

void
son(char *filename)
{
    char command[1000];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        _exit(1);
    }
    if (fgets(command, sizeof(command), file) == NULL) {
        fclose(file);
        _exit(1);
    }
    char *newline = strchr(command, '\n');
    *newline = '\0';
    fclose(file);
    execlp(command, command, NULL);
    _exit(1);
}

int
main(int argc, char **argv)
{
    int counter = 0;
    char *endptr;
    int N = (int) strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || (*endptr != '\0' && *endptr != '\n')) {
        _exit(1);
    }

    for (int i = 2; i < N + 2; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            continue;
        } else if (pid == 0) {
            son(argv[i]);
        }
    }
    for (int i = 0; i < N; i++) {
        int status = 0;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            counter += 1;
        }
    }

    for (int i = N + 2; i < argc; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            continue;
        } else if (pid == 0) {
            son(argv[i]);
        }

        int status = 0;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            counter += 1;
        }
    }

    printf("%d\n", counter);
    return 0;
}
