#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

//made by: ded

enum
{
    STDIN_FD = 0,
    STDOUT_FD = 1
};

void
kill_sons(pid_t *pid, int len)
{
    for (int i = 0; i < len; i++) {
        if (pid[i] <= 0) {
            break;
        }
        kill(pid[i], SIGKILL);
    }
    for (int i = 0; i < len; i++) {
        wait(NULL);
    }
    return;
}

int
main(int argc, char **argv)
{
    if (argc == 1) {
        return 0;
    }
    int com_num = argc - 1;
    pid_t *pid = malloc(sizeof(pid_t) * com_num);

    if (pid == NULL) {
        return 1;
    }

    if (memset(pid, 0, sizeof(pid_t) * com_num) != pid) {
        free(pid);
        return 1;
    }

    int prev_pipe[2] = {-1, -1};
    int cur_pipe[2] = {-1, -1};
    int flag = 0;
    for (int i = 0; i < argc - 1; i++) {
        if (pipe(cur_pipe) < 0) {
            flag = 1;
            _exit(1);
        }
        if ((pid[i] = fork()) == 0) {
            if (prev_pipe[1] != -1) {
                close(prev_pipe[1]);
            }

            if (cur_pipe[0] != -1) {
                close(cur_pipe[0]);
            }

            if (i > 0) {
                if (dup2(prev_pipe[0], STDIN_FD) < 0) {
                    _exit(1);
                }
            }
            if (i < com_num - 1) {
                if (dup2(cur_pipe[1], STDOUT_FD) < 0) {
                    _exit(1);
                }
            }

            if (prev_pipe[0] != -1) {
                close(prev_pipe[0]);
            }

            if (cur_pipe[1] != -1) {
                close(cur_pipe[1]);
            }

            execlp(argv[i + 1], argv[i + 1], NULL);
            _exit(1);
        }

        if (pid[i] < 0) {
            flag = 1;
            break;
        }

        if (prev_pipe[0] != -1) {
            close(prev_pipe[0]);
        }
        if (prev_pipe[1] != -1) {
            close(prev_pipe[1]);
        }

        prev_pipe[0] = cur_pipe[0];
        prev_pipe[1] = cur_pipe[1];
    }

    close(cur_pipe[0]);
    close(cur_pipe[1]);

    if (flag == 1) {
        kill_sons(pid, argc - 1);
        return 1;
    }

    for (int i = 0; i < argc - 1; i++) {
        wait(NULL);
    }

    free(pid);
    return 0;
}
