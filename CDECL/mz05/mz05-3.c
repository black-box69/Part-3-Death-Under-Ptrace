#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//made by: CDECL

int
parse_rwx_permissions(const char *str)
{
    const char FORMAT[] = "rwxrwxrwx";
    if (str == NULL) {
        return -1;
    }
    int form_size = sizeof(FORMAT) - 1;
    int str_size = strlen(str);
    if (form_size != str_size) {
        return -1;
    }
    unsigned mask = 1 << (form_size - 1);
    unsigned res = 0;
    for (int i = 0; i < form_size; i++) {
        if (str[i] == FORMAT[i]) {
            res += mask;
        } else if (str[i] != '-') {
            return -1;
        }
        mask >>= 1;
    }
    return res;
}

