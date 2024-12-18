#include <time.h>
#include <stdio.h>
#include <string.h>

//made by: ded

int
main(int argc, char **argv)
{
    int year = 0;
    scanf("%d", &year);
    year -= 1900;
    for (int month = 0; month < 12; month++) {
        int week = 0;
        for (int day = 1; day <= 31; day++) {
            struct tm time = {0};
            time.tm_year = year;
            time.tm_mon = month;
            time.tm_mday = day;
            time.tm_isdst = -1;

            if (mktime(&time) == -1 || time.tm_mon != month) {
                continue;
            }

            if (time.tm_wday == 4) {
                week++;
                if ((week % 2 == 0) && (day % 3 != 0)) {
                    printf("%d %d\n", month + 1, day);
                }
            }
        }
    }
    return 0;
}
