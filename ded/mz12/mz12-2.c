#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

//made by: ded

int
func(char *file)
{
    pid_t pid = 0;
    int status = 0;
    if ((pid = fork()) == 0) {
        execlp(file, file, NULL);
        _exit(1);
    }
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        return 1;
    }
    return 0;
}

int
main(int argc, char **argv)
{
    if (argc != 4) {
        return 0;
    }

    int flag = 0;
    if (func(argv[1])) {
        flag = 1;
    } else if (func(argv[2])) {
        flag = 1;
    }
    int res = 0;
    if (flag) {
        res = func(argv[3]);
    }

    if (res) {
        return 0;
    }
    return 1;
}
