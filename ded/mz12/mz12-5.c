#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

//made by: ded

enum
{
    BUFSIZE = 256
};

const char DEF_TMP[] = "/tmp";
const char XDG_DIR[] = "XDG_RUNTIME_DIR";
const char TMPDIR[] = "TMPDIR";
const char CRYPTOKEY[] = "/dev/urandom";

char c_prog[] = "#include <stdio.h>\n"
                "#include <stdlib.h>\n"
                "#include <unistd.h>\n"
                "char summon[] = \"summon\";\n"
                "char disqualify[] = \"disqualify\";\n"
                "char reject[] = \"reject\";\n"
                "char *calc(int x) { return %s; }\n"
                "int main(void) {\n"
                "int tmp = 0;\n"
                "while (scanf(\"%%d\", &tmp) != EOF) {\n"
                "char *result = calc(tmp);"
                "if (result == NULL) {"
                "fprintf(stderr, \"evaluate returned NULL\\n\");\n"
                "continue;\n"
                "}\n"
                "printf(\"%%s\\n\", result);\n"
                "}\n"
                "unlink(\"%s\");\n"
                "unlink(\"%s\");\n"
                "}\n";

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
    char PROG_NAME[BUFSIZE];

    int rand_fd = open(CRYPTOKEY, O_RDONLY);

    unsigned rand_int_1 = 0;
    unsigned rand_int_2 = 0;

    if (read(rand_fd, &rand_int_1, sizeof(rand_int_1)) < 0) {
        close(rand_fd);
        exit(1);
    }

    if (read(rand_fd, &rand_int_2, sizeof(rand_int_2)) < 0) {
        close(rand_fd);
        exit(1);
    }

    close(rand_fd);

    sprintf(FILE_NAME, "%s/%u.c", FILE_PATH, rand_int_1);
    sprintf(PROG_NAME, "%s/%u", FILE_PATH, rand_int_2);

    FILE *c_lib = fopen(FILE_NAME, "w");

    if (c_lib == NULL) {
        unlink(FILE_NAME);
        close(rand_fd);
        exit(1);
    }

    if (chmod(FILE_NAME, S_IRUSR | S_IWUSR) != 0) {
        fclose(c_lib);
        exit(1);
    }

    fprintf(c_lib, c_prog, argv[1], FILE_NAME, PROG_NAME);

    fclose(c_lib);

    pid_t pid = fork();

    if (pid < 0) {
        exit(1);
    } else if (pid == 0) {
        execlp("gcc", "gcc", FILE_NAME, "-o", PROG_NAME, NULL);
        exit(1);
    } else {
        int status;
        wait(&status);

        if (!(WIFEXITED(status) && WEXITSTATUS(status) == 0)) {
            unlink(FILE_NAME);
            exit(1);
        }
    }

    if (chmod(PROG_NAME, S_IXUSR) != 0) {
        unlink(FILE_NAME);
        unlink(PROG_NAME);
        exit(1);
    }

    execl(PROG_NAME, PROG_NAME, NULL);
    exit(1);
}
