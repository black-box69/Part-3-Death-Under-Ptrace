#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

int
main(void)
{
    double x, y;
    scanf("%lf%lf", &x, &y);
    if (x >= 2 && x <= 5 && y >= 1 && y <= 7 && y >= x - 2) {
        printf("%d", 1);
    } else {
        printf("%d", 0);
    }
    return 0;
}

