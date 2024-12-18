#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

//made by: CDECL

enum
{
    START = 'a',
    END = 'z'
};

int main(int argc, char **argv) {
    int cur = 0;
    int bias = 13;
    while ((cur = getchar_unlocked()) != EOF) {
        if (START <= cur && cur <= END) {
            cur = (((cur - START) + bias) % (END - START + 1)) + START;
            printf("%c", (char) cur);
        } else if (cur == (int) '\n') {
            printf("%c", (char) cur);
            bias--;
            if (bias < 0) {
                bias = 25;
            }
        } else {
            printf("%c", (char) cur);
        }
    }
    return 0;
}

