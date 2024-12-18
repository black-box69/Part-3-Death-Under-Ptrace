#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

//made by: ded

int
main(int argc, char **argv)
{
    int pipefd[2];
    pipe(pipefd);
    pid_t pid = -1;
    if ((pid = fork()) == 0) {
        pid_t pid2 = -1;
        if ((pid2 = fork()) == 0) {
            pid_t pid3 = -1;
            if ((pid3 = fork()) == 0) {
                close(pipefd[0]);
                time_t cur_time = time(NULL);
                for (int i = 0; i < 3; i++) {
                    write(pipefd[1], &cur_time, sizeof(cur_time));
                }
                _exit(0);
            } else {
                if (pid3 < 0) {
                    close(pipefd[1]);
                    close(pipefd[0]);
                    _exit(1);
                }
                close(pipefd[1]);
                wait(NULL);
                time_t grandson_time = 0;
                read(pipefd[0], &grandson_time, sizeof(grandson_time));
                close(pipefd[0]);
                struct tm *grandson_time_tm = localtime(&grandson_time);
                printf("D:%.2d\n", grandson_time_tm->tm_mday);
                fflush(stdout);
                _exit(0);
            }
        } else {
            if (pid2 < 0) {
                close(pipefd[1]);
                close(pipefd[0]);
                _exit(1);
            }
            close(pipefd[1]);
            wait(NULL);
            time_t son_time = 0;
            read(pipefd[0], &son_time, sizeof(son_time));
            close(pipefd[0]);
            struct tm *son_time_tm = localtime(&son_time);
            printf("M:%.2d\n", son_time_tm->tm_mon + 1);
            fflush(stdout);
            _exit(0);
        }
    } else {
        if (pid < 0) {
            close(pipefd[1]);
            close(pipefd[0]);
            _exit(1);
        }
        close(pipefd[1]);
        wait(NULL);
        time_t father_time = 0;
        read(pipefd[0], &father_time, sizeof(father_time));
        close(pipefd[0]);
        struct tm *father_time_tm = localtime(&father_time);
        printf("Y:%.4d\n", father_time_tm->tm_year + 1900);
        fflush(stdout);
    }
    return 0;
}
