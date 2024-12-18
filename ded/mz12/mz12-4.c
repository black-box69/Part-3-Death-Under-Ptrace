#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

//made by: ded

enum
{
    BUFSIZE = 256
};

const char DEF_TMP[] = "/tmp";
const char XDG_DIR[] = "XDG_RUNTIME_DIR";
const char TMPDIR[] = "TMPDIR";
const char CRYPTOKEY[] = "/dev/urandom";

int
main(int argc, char **argv)
{
    if (argc < 2) {
        exit(1);
    }

    char *FILE_PATH = getenv(XDG_DIR);
    if (FILE_PATH == NULL || access(FILE_PATH, W_OK)) {
        FILE_PATH = getenv(TMPDIR);
        if (FILE_PATH == NULL || access(FILE_PATH, W_OK)) {
            FILE_PATH = strdup(DEF_TMP);
        }
    }

    char FILE_NAME[BUFSIZE];

    int rand_fd = open(CRYPTOKEY, O_RDONLY);

    unsigned rand_int_1 = 0;

    if (read(rand_fd, &rand_int_1, sizeof(rand_int_1)) < 0) {
        close(rand_fd);
        exit(1);
    }

    sprintf(FILE_NAME, "%s/%d.py", FILE_PATH, rand_int_1);

    FILE *python = fopen(FILE_NAME, "w");

    if (chmod(FILE_NAME, S_IRWXU) != 0) {
        fclose(python);
        exit(1);
    }

    if (fprintf(python, "#! /usr/bin/python3\n") < 0) {
        exit(1);
    }
    if (fprintf(python, "print(%s ", argv[1]) < 0) {
        exit(1);
    }

    for (int i = 2; i < argc; i++) {
        if (fprintf(python, " * %s", argv[i]) < 0) {
            exit(1);
        }
    }

    if (fprintf(python, ")\n") < 0) {
        exit(1);
    }

    fclose(python);

    char command[1000];

    sprintf(command, "python3 %s", FILE_NAME);

    if (system(command) < 0) {
        exit(1);
    }

    if (unlink(FILE_NAME) != 0) {
        exit(1);
    }
}
