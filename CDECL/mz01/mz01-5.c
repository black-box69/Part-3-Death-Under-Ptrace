#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

void
gen(int i, int n, int *perm, int *used)
{
    if (i == n) {
        for (int j = 0; j < n; j++) {
            printf("%d", perm[j]);
        }
        printf("\n");
    } else {
        for (int j = 0; j < n; j++) {
            if (!used[j]) {
                perm[i] = j + 1;
                used[j] = 1;
                gen(i + 1, n, perm, used);
                used[j] = 0;
            }
        }
    }
}

int
main(void)
{
    int n;
    if (scanf("%d", &n) != 1) {
        exit(1);
    }
    if (n <= 0 || n >= 10) {
        exit(1);
    }
    int perm[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int used[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    gen(0, n, perm, used);
    return 0;
}

