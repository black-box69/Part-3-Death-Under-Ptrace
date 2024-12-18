#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

//made by: CDECL

int
main(void)
{
    int year;
    if (scanf("%d", &year) == -1) {
        exit(1);
    }
    if (year < 1900 || year > 2037) {
        exit(1);
    }
    year -= 1900;
    struct tm tt;
    memset(&tt, 0, sizeof(tt));
    tt.tm_isdst = -1;
    tt.tm_year = year;
    tt.tm_hour = 12;
    tt.tm_mday = 1;
    mktime(&tt);
    tt.tm_isdst = -1;
    int delta = (4 - tt.tm_wday + 7) % 7;
    tt.tm_mday += delta;
    int cur_month = 0;
    int th_num = 0;
    int day_offset = tt.tm_mday;
    mktime(&tt);
    while (tt.tm_year == year) {
        tt.tm_isdst = -1;
        if (cur_month == tt.tm_mon) {
            if (th_num) {
                if (tt.tm_mday % 3 != 0) {
                    printf("%d %d\n", tt.tm_mon + 1, tt.tm_mday);
                }
            }
        } else {
            cur_month = tt.tm_mon;
            th_num = 0;
            day_offset = tt.tm_mday;
        }
        th_num = !th_num;
        day_offset += 7;
        tt.tm_mday = day_offset;
        mktime(&tt);
    }
    return 0;
}

