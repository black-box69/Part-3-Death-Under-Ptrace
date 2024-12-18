#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

//made by: CDECL

enum
{
    NAME_LEN = 16,
    ARGC_N = 2
};

const char comm[] = "#!/usr/bin/env python3\n";
const char pyscript_start[] = "from os import remove\nprint(";
const char pyscript_end[] = ")\nremove(\"%s\")\n";
const char tmp_t1[] = "XDG_RUNTIME_DIR";
const char tmp_t2[] = "TMPDIR";
const char tmp_t3[] = "/tmp";
const char rand_digit[] = "/dev/urandom";

int
main(int argc, char **argv)
{
    if (argc < ARGC_N) {
        exit(1);
    }
    int rfd;
    if ((rfd = open(rand_digit, O_RDONLY)) == -1) {
        exit(1);
    }
    unsigned num = 0;
    if (read(rfd, &num, sizeof(num)) == -1) {
        exit(1);
    }
    close(rfd);
    // unsigned num = (unsigned) getpid();
    char *tmp_dir;
    if (!(tmp_dir = getenv(tmp_t1)) || access(tmp_dir, W_OK)) {
        if (!(tmp_dir = getenv(tmp_t2)) || access(tmp_dir, W_OK)) {
            tmp_dir = strdup(tmp_t3);
        }
    }
    char *name = calloc(NAME_LEN, sizeof(*name));
    if (snprintf(name, NAME_LEN, "%u.py", num) == -1) {
        exit(1);
    }
    char *path = calloc(PATH_MAX, sizeof(*path));
    if (snprintf(path, PATH_MAX, "%s/%u.py", tmp_dir, num) == -1) {
        exit(1);
    }
    int exec_file = open(path, O_WRONLY | O_CREAT | O_EXCL, 0700);
    if (exec_file == -1) {
        exit(1);
    }
    if (dprintf(exec_file, "%s", comm) == -1) {
        exit(1);
    }
    if (dprintf(exec_file, "%s", pyscript_start) == -1) {
        exit(1);
    }
    if (dprintf(exec_file, "%s", argv[1]) == -1) {
        exit(1);
    }
    for (int i = 2; i < argc; i++) {
        if (dprintf(exec_file, " * %s", argv[i]) == -1) {
            exit(1);
        }
    }
    if (dprintf(exec_file, pyscript_end, path) == -1) {
        exit(1);
    }
    close(exec_file);
    chmod(path, 0700);
    execl(path, name, NULL);
    exit(0);
}

