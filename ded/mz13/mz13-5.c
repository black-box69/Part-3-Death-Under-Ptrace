#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//made by: ded

void
create_proc(int proc_it, int trashhold, int readfd, int writefd)
{
    FILE *r_stream = fdopen(readfd, "r");
    FILE *w_stream = fdopen(writefd, "w");
    if (!r_stream || !w_stream) {
        fflush(stdout);
        _exit(0);
    }

    int num = 0;
    while (1) {
        fflush(stdout);
        if (fscanf(r_stream, "%d", &num) != 1) {
            break;
        }
        if (num >= trashhold) {
            break;
        }
        printf("%d %d\n", proc_it, num);
        fflush(stdout);
        fprintf(w_stream, "%d\n", num + 1);
        fflush(w_stream);
    }
    fclose(r_stream);
    fclose(w_stream);
    _exit(0);
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    int pipefd_12[2];
    int pipefd_21[2];

    pipe(pipefd_12);
    pipe(pipefd_21);

    int trashhold = atoi(argv[1]);

    if (fork() == 0) {
        close(pipefd_12[0]);
        close(pipefd_21[1]);
        create_proc(1, trashhold, pipefd_21[0], pipefd_12[1]);
    }

    if (fork() == 0) {
        close(pipefd_21[0]);
        close(pipefd_12[1]);
        create_proc(2, trashhold, pipefd_12[0], pipefd_21[1]);
    }

    FILE *w_stream = fdopen(pipefd_21[1], "w");
    fprintf(w_stream, "%d\n", 1);
    fflush(w_stream);
    fclose(w_stream);

    close(pipefd_12[0]);
    close(pipefd_21[0]);
    close(pipefd_12[1]);

    wait(NULL);
    wait(NULL);
    printf("Done\n");
    fflush(stdout);

    return 0;
}
