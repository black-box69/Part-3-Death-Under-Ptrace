#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

int
is_prime(int n)
{
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) {
            return 0;
        }
        i++;
    }
    return 1;
}

int
main(void)
{
    int n;
    if (scanf("%d", &n) != 1 || n <= 0 || !is_prime(n) || n >= 2000) {
        fprintf(stderr, "invalid input ");
        exit(1);
    }
    int *arr = calloc(n - 1, sizeof(*arr));
    if (!arr) {
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            if (i * j % n == 1) {
                arr[j - 1] = i;
                break;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            printf("%d ", i * arr[j] % n);
        }
        printf("\n");
    }
    free(arr);
    return 0;
}

