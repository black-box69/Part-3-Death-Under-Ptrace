#include <stdio.h>
#include <string.h>

//made by: ded

const char perm_string[] = "rwxrwxrwx";

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }

    int res_num = 0;

    int str_size = strlen(str);

    if (str_size != 9) {
        return -1;
    }

    for (int i = 0; i < sizeof(perm_string) - 1; i++) {
        res_num = res_num << 1;
        if (str[i] == perm_string[i]) {
            res_num += 1;
        } else if (str[i] != '-') {
            return -1;
        }
    }
    return res_num;
}
