#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>

//made by: ded

int
main(int argc, char**argv)
{
    char read_elem = 0;
    int check_err = 0;
    int K = 13;
    while ((check_err = scanf("%c", &read_elem)) == 1) {
        if (islower(read_elem)) {
            printf("%c", 'a' + (read_elem - 'a' + K) % ('z'-'a' + 1));
        } else if (read_elem == '\r') {
            continue;
        } else if (read_elem == '\n') {
            K--;
            printf("\n");
        } else {
            printf("%c", read_elem);
        }
        if (K == -1) {
            K = 25;
        }
    }
    if(read_elem != '\n') {
        printf("\n");
    }
    return 0;
}
