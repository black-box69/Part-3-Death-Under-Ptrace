#include <stdio.h>
#include <stdlib.h>

//made by: ded

#define eps 1e-6

int
main(int argc, char *argv[])
{
    double x, y;
    scanf("%lf%lf", &x, &y);
    if (x < 2 || x > 5 || y > 7 || y < 1) {
        printf("0\n");
        return 0;
    }
    if (y < x - 2) {
        printf("0\n");
        return 0;
    }
    printf("1\n");
    return 0;
}
