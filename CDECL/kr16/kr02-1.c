#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//made by: CDECL

enum {
    MASK = 0x3
};

int main(int argc, char **argv) {
    unsigned long long cur;
    unsigned long long ans = 0;
    while (scanf("%llx", &cur) == 1) {
        for (int i = 0; i < 32; i++) {
            if (((cur >> i * 2) & MASK) == 0) {
                ans++;
            }
        }
    }
    printf("%llu\n", ans);
}

