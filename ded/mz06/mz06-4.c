#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

//made by: ded

void
recursive_print(const char *dir, const char *rel_path, long long limit_file_size, int depth)
{
    if (depth == 0) {
        return;
    }

    if (access(dir, R_OK | X_OK | F_OK)) {
        return;
    }

    DIR *dirp = opendir(dir);
    if (dirp == NULL) {
        return;
    }

    struct dirent *dir_entry;
    while ((dir_entry = readdir(dirp))) {
        if (strcmp(".", dir_entry->d_name) == 0 || strcmp("..", dir_entry->d_name) == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir, dir_entry->d_name);

        char new_rel_path[PATH_MAX];
        if (rel_path[0] == '\0') {
            snprintf(new_rel_path, sizeof(new_rel_path), "%s", dir_entry->d_name);
        } else {
            snprintf(new_rel_path, sizeof(new_rel_path), "%s/%s", rel_path, dir_entry->d_name);
        }

        struct stat stb;
        if (lstat(path, &stb) < 0) {
            continue;
        }

        if (S_ISLNK(stb.st_mode)) {
            continue;
        }

        if (S_ISDIR(stb.st_mode)) {
            recursive_print(path, new_rel_path, limit_file_size, depth - 1);
        } else if (S_ISREG(stb.st_mode)) {
            if (access(path, R_OK) == 0 && stb.st_size <= limit_file_size) {
                printf("%s\n", new_rel_path);
            }
        }
    }
    closedir(dirp);
}

int
main(int argc, char **argv)
{
    if (argc < 3) {
        return 0;
    }
    const char *D = argv[1];
    long long Z = atoll(argv[2]);

    recursive_print(D, "", Z, 4);
    return 0;
}
