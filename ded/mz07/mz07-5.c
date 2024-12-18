#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//made by: ded

enum
{
    MOON_YEAR = 2021 - 1900,
    MOON_MON = 4,
    MOON_DAY = 26,
    MOON_HOUR = 10,
    MOON_MIN = 13,
    MOON_SEC = 0
};

enum
{
    SYNOD_DAY = 29,
    SYNOD_HOUR = 12,
    SYNOD_MIN = 44
};

void
find_holiday(int year)
{
    year -= 1900;
    struct tm moon_cntr;
    memset(&moon_cntr, 0, sizeof(moon_cntr));
    moon_cntr.tm_year = MOON_YEAR;
    moon_cntr.tm_mon = MOON_MON;
    moon_cntr.tm_mday = MOON_DAY;
    moon_cntr.tm_hour = MOON_HOUR;
    moon_cntr.tm_min = MOON_MIN;
    moon_cntr.tm_sec = MOON_SEC;
    moon_cntr.tm_isdst = -1;

    errno = 0;
    int check_err = mktime(&moon_cntr);
    if (errno && check_err == -1) {
        exit(errno);
    }

    if (moon_cntr.tm_year <= year) {
        while (moon_cntr.tm_year != year || moon_cntr.tm_yday <= 255) {
            moon_cntr.tm_mday += SYNOD_DAY;
            moon_cntr.tm_hour += SYNOD_HOUR;
            moon_cntr.tm_min += SYNOD_MIN;
            moon_cntr.tm_isdst = -1;
            errno = 0;

            int check_err = mktime(&moon_cntr);
            if (errno && check_err == -1) {
                exit(errno);
            }
        }
    } else {
        while (moon_cntr.tm_year != year || moon_cntr.tm_yday > 255) {
            moon_cntr.tm_mday -= SYNOD_DAY;
            moon_cntr.tm_hour -= SYNOD_HOUR;
            moon_cntr.tm_min -= SYNOD_MIN;
            moon_cntr.tm_isdst = -1;
            errno = 0;

            int check_err = mktime(&moon_cntr);
            if (errno && check_err == -1) {
                exit(errno);
            }
        }
        moon_cntr.tm_mday += SYNOD_DAY;
        moon_cntr.tm_hour += SYNOD_HOUR;
        moon_cntr.tm_min += SYNOD_MIN;
        moon_cntr.tm_isdst = -1;
        errno = 0;

        int check_err = mktime(&moon_cntr);
        if (errno && check_err == -1) {
            exit(errno);
        }
    }

    moon_cntr.tm_mday += 1;
    int monday_cntr = 0;
    while (monday_cntr < 4) {
        errno = 0;
        int check_err = mktime(&moon_cntr);
        if (errno && check_err == -1) {
            exit(errno);
        }

        if (moon_cntr.tm_wday == 1) {
            monday_cntr += 1;
        }
        if (monday_cntr == 4) {
            break;
        }
        moon_cntr.tm_mday += 1;
        moon_cntr.tm_isdst = -1;
    }
    printf("%d-%02d-%02d\n", moon_cntr.tm_year + 1900, moon_cntr.tm_mon + 1, moon_cntr.tm_mday);
    return;
}

int
main(int argc, char **argv)
{
    int year = 0;
    scanf("%d", &year);
    find_holiday(year);
    return 0;
}
