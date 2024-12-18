#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

//made by: ded

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    if (buf == NULL && size > 0) {
        return -1;
    }

    char tmp_buf[PATH_MAX];
    ssize_t pointer = PATH_MAX - 1;
    tmp_buf[pointer] = '\0';
    ssize_t path_len = 0;

    int orig_fd = open(".", O_RDONLY);
    if (orig_fd == -1) {
        return -1;
    }

    if (fchdir(fd) == -1) {
        close(orig_fd);
        return -1;
    }

    struct stat cur_st, par_st;
    if (stat(".", &cur_st) == -1) {
        fchdir(orig_fd);
        close(orig_fd);
        return -1;
    }

    while (1) {
        if (stat("..", &par_st) == -1) {
            fchdir(orig_fd);
            close(orig_fd);
            return -1;
        }
        if (cur_st.st_ino == par_st.st_ino && cur_st.st_dev == par_st.st_dev) {
            if (pointer == PATH_MAX - 1) {
                if (pointer < 1) {
                    fchdir(orig_fd);
                    close(orig_fd);
                    return -1;
                }
                tmp_buf[--pointer] = '/';
                path_len = 1;
            }
            break;
        }

        if (chdir("..") == -1) {
            fchdir(orig_fd);
            close(orig_fd);
            return -1;
        }

        DIR *dir = opendir(".");
        if (!dir) {
            fchdir(orig_fd);
            close(orig_fd);
            return -1;
        }

        struct dirent *entry;
        int found = 0;
        while ((entry = readdir(dir)) != NULL) {
            struct stat entry_st;
            if (lstat(entry->d_name, &entry_st) == -1) {
                continue;
            }
            if (entry_st.st_ino == cur_st.st_ino && entry_st.st_dev == cur_st.st_dev) {
                size_t name_len = strlen(entry->d_name);
                if (pointer < name_len + 1) {
                    closedir(dir);
                    fchdir(orig_fd);
                    close(orig_fd);
                    return -1;
                }
                pointer -= name_len;
                memcpy(tmp_buf + pointer, entry->d_name, name_len);
                tmp_buf[--pointer] = '/';
                path_len += name_len + 1;
                found = 1;
                break;
            }
        }
        closedir(dir);

        if (!found) {
            fchdir(orig_fd);
            close(orig_fd);
            return -1;
        }
        cur_st = par_st;
    }

    if (fchdir(orig_fd) == -1) {
        close(orig_fd);
        return -1;
    }
    close(orig_fd);

    if (size > 0) {
        ssize_t copy_len = (path_len < size - 1) ? path_len : size - 1;
        memcpy(buf, tmp_buf + pointer, copy_len);
        buf[copy_len] = '\0';
    }

    return path_len;
}
