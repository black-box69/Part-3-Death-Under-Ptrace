#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//made by: CDECL

void
normalize_path(char *buf)
{
    if (!buf) {
        return;
    }
    int i = 0, j = 0, n = 0;
    while (buf[n] != '\0') {
        n++;
    }
    while (i < n) {
        if (buf[i] == '/') {
            if (i + 3 <= n && buf[i + 1] == '.' && buf[i + 2] == '.' && (buf[i + 3] == '/' || buf[i + 3] == '\0')) {
                if (j != 0) {
                    j--;
                    while (j > 0 && buf[j] != '/') {
                        j--;
                    }
                }
                i += 3;
            } else if (i + 2 <= n && buf[i + 1] == '.' && (buf[i + 2] == '/' || buf[i + 2] == '\0')) {
                i += 2;
            } else if (i + 1 <= n && (buf[i + 1] == '/' || buf[i + 1] == '\0')) {
                i++;
            } else {
                buf[j] = '/';
                i++;
                j++;
            }
        } else {
            buf[j] = buf[i];
            i++;
            j++;
        }
    }
    if (j == 0) {
        buf[j + 1] = '\0';
    } else {
        buf[j] = '\0';
    }
}

char *
relativize_path(const char *path1, const char *path2)
{
    const char prev[] = "../";
    char *p1 = strdup(path1);
    char *p2 = strdup(path2);
    normalize_path(p1);
    normalize_path(p2);
    int len1 = strlen(p1), len2 = strlen(p2), len_prev = sizeof(prev) - 1;
    int left_cur = 0, right_cur = len1 - 1;
    char *res = calloc(sizeof(*res), 2 * (len1 + len2) + 1);
    while (p1[right_cur] != '/') {
        right_cur--;
    }
    for (int i = 0; i <= right_cur; i++) {
        if (p1[i] == '/') {
            left_cur = i;
        }
        if (p1[i] != p2[i]) {
            if ((p1[i] == '/' && p2[i] == '\0') || (p1[i] == '\0' && p2[i] == '/')) {
                left_cur = i;
            }
            break;
        }
    }
    int write_cur = 0;
    for (int i = left_cur; i < right_cur; i++) {
        if (p1[i] == '/') {
            for (int j = 0; j < len_prev; j++) {
                res[write_cur + j] = prev[j];
            }
            write_cur += len_prev;
        }
    }
    for (int i = left_cur + 1; i <= len2; i++) {
        res[write_cur] = p2[i];
        write_cur++;
    }
    int len_res = strlen(res);
    if (len_res != 0 && res[len_res - 1] == '/') {
        res[len_res - 1] = '\0';
    }
    if (write_cur == 1 || write_cur == 0) {
        res[0] = '.';
    }
    free(p1);
    free(p2);
    return res;
}

