#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <time.h>

//made by: CDECL

enum
{
    NAME_LEN = 16,
    ARGC_N = 2
};

const char code[] =
    "\n#include <stdio.h>\n#include <unistd.h>\nconst char summon[] = \"summon\";\nconst char reject[] = \"reject\"\
            ;\nconst char disqualify[] = \"disqualify\";\nint main(void) {\nint x = 0;\nwhile \
            (scanf(\"%%d\", &x) == 1) {\nprintf(\"%%s\\n\", %s);\n}\nunlink(\"%s\");\n}";

const char tmp_t1[] = "XDG_RUNTIME_DIR";
const char tmp_t2[] = "TMPDIR";
const char tmp_t3[] = "/tmp";
const char rand_digit[] = "/dev/urandom";

int
main(int argc, char **argv)
{
    if (argc != ARGC_N) {
        exit(1);
    }
    int rfd = open(rand_digit, O_RDONLY);
    if (rfd == -1) {
        exit(1);
    }
    unsigned num;
    if (read(rfd, &num, sizeof(num)) == -1) {
        exit(1);
    }
    close(rfd);
    char *tmp_dir;
    if (!(tmp_dir = getenv(tmp_t1))) {
        if (!(tmp_dir = getenv(tmp_t2))) {
            tmp_dir = strdup(tmp_t3);
        }
    }
    char *file_name = calloc(NAME_LEN, sizeof(*file_name));
    snprintf(file_name, NAME_LEN, "%u.c", num);
    char *exec_name = calloc(PATH_MAX, sizeof(*exec_name));
    snprintf(exec_name, PATH_MAX, "%s/%u", tmp_dir, num);
    char *path = calloc(PATH_MAX, sizeof(*path));
    snprintf(path, PATH_MAX, "%s/%s", tmp_dir, file_name);
    FILE *exec_file = fopen(path, "w");
    fprintf(exec_file, code, argv[1], path);
    fclose(exec_file);
    pid_t pid;
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (!pid) {
        execlp("gcc", "gcc", path, "-o", exec_name, NULL);
        _exit(1);
    }
    int status;
    wait(&status);
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        execlp(exec_name, exec_name, NULL);
    }
    _exit(1);
}

