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

//made by: ded

int
main(int argc, char**argv)
{
    int i = argc-1;
    int cnt = 0;
    int num = 0;
    unsigned long long uns_res = 0;
    while (i > 1) {
        sscanf(argv[i], "%d", &cnt);
        sscanf(argv[i-1], "%d", &num);
        for(int j = 0; j < cnt; j++) {
            uns_res = uns_res << 1;
            uns_res += num;
        }
        i -= 2;
    }
    long long res = (long long) uns_res;
    printf("%lld\n", res);
    return 0;
}
