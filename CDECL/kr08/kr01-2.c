#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

int main(int argc, char **argv) {
    unsigned long long res = 0;
    unsigned long long mod = 0;
    unsigned counter = 0;
    for (int i = argc - 1; i > 0; i -= 2) {
        sscanf(argv[i - 1], "%llu", &mod);
        sscanf(argv[i], "%u", &counter);
        for (; counter != 0; counter--) {
            res <<= 1;
            res ^= mod;
        }
    }
    printf("%lld\n", (long long) res);
    return 0;
}
