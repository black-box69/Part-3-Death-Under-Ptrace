#include <stdio.h>

//made by: ded

void
normalize_path(char *buf)
{
    if (buf[1] == '\0') {
        return;
    }
    char *dst = buf + 1;
    char *src = buf + 1;
    while (src[0] != '\0') {
        if (src[0] == '.' && (src[1] == '/' || src[1] == '\0')) {
            while (src[0] != '/' && src[0] != '\0') {
                ++src;
            }
            if (*src == '/') {
                ++src;
            }
            continue;
        }
        if (src[0] == '.' && src[1] == '.' && (src[2] == '/' || src[2] == '\0')) {
            src += 2;
            if (*src == '/') {
                ++src;
            }
            if (dst > buf + 1) {
                --dst;
                while (dst > buf && *(dst - 1) != '/') {
                    --dst;
                }
            }
            continue;
        }
        while (src[0] != '/' && src[0] != '\0') {
            *dst = *src;
            ++dst;
            ++src;
        }
        if (src[0] == '/') {
            *dst = *src;
            ++dst;
            ++src;
        }
    }
    if (dst > buf + 1 && *(dst - 1) == '/') {
        dst--;
    }
    dst[0] = '\0';
    return;
}
