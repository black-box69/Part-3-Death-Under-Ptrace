#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//made by: CDECL

const char ERR[] = "18446744073709551616";

int
main(void)
{
    int cur;
    while ((cur = getchar_unlocked()) != EOF && isspace(cur))
        ;
    long long res = 0;
    long long overflow = 0;
    char sym;
    long long s = 0;
    long long f = 0;
    long long tmp = 0;
    while (cur != EOF) {
        sym = cur;
        if (isspace(cur)) {
            if (!f) {
                if (overflow) {
                    printf("%s\n", ERR);
                } else {
                    printf("%lld\n", res);
                }
                res = 0;
                overflow = 0;
                f = 1;
            }
        } else {
            s = 1;
            f = 0;
            tmp = 0;
            if (sym == '1') {
                if (res < 0) {
                    overflow |= __builtin_mul_overflow(res + 1, 3, &tmp);
                    overflow |= __builtin_sub_overflow(tmp, 2, &res);
                } else {
                    overflow |= __builtin_mul_overflow(res, 3, &tmp);
                    overflow |= __builtin_add_overflow(tmp, 1, &res);
                }
            } else if (sym == 'a') {
                if (res > 0) {
                    overflow |= __builtin_mul_overflow(res - 1, 3, &tmp);
                    overflow |= __builtin_add_overflow(tmp, 2, &res);
                } else {
                    overflow |= __builtin_mul_overflow(res, 3, &tmp);
                    overflow |= __builtin_sub_overflow(tmp, 1, &res);
                }
            } else if (sym == '0') {
                overflow |= __builtin_mul_overflow(res, 3, &tmp);
                res = tmp;
            }
        }
        cur = getchar_unlocked();
    }
    if (cur == EOF && !f && s) {
        if (!overflow) {
            printf("%lld\n", res);
        } else {
            printf("%s\n", ERR);
        }
    }
}

