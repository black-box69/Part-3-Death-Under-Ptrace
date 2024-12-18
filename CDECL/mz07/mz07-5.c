#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//made by: CDECL

enum
{
    COR_YDAY = 255,
    MOON_YEAR = 2021,
    MOON_MON = 4,
    MOON_MDAY = 26,
    MOON_HOUR = 11,
    MOON_MIN = 14,
    INT_DAY = 29,
    INT_HOUR = 12,
    INT_MIN = 44,
    COR_NUM = 4,
    COR_WDAY = 1,
    START_YEAR = 1902,
    END_YEAR = 2037,
    OFFSET = 1900,
    WEEK = 7,
    NO_INFO = -1
};

const char TEMP[] = "%Y-%m-%d";

int
main(void)
{
    int year;
    if (scanf("%d", &year) != 1) {
        exit(1);
    }
    if (year < START_YEAR || year > END_YEAR) {
        exit(1);
    }
    struct tm tt = {0};
    tt.tm_isdst = NO_INFO;
    tt.tm_year = MOON_YEAR - OFFSET;
    tt.tm_mon = MOON_MON;
    tt.tm_mday = MOON_MDAY;
    tt.tm_hour = MOON_HOUR;
    tt.tm_min = MOON_MIN;
    mktime(&tt);
    tt.tm_isdst = NO_INFO;
    char ans[20];
    if (year < MOON_YEAR) {
        while (tt.tm_year != year - OFFSET || tt.tm_yday > COR_YDAY) {
            tt.tm_mday -= INT_DAY;
            tt.tm_hour -= INT_HOUR;
            tt.tm_min -= INT_MIN;
            mktime(&tt);
            tt.tm_isdst = NO_INFO;
        }
        tt.tm_mday += INT_DAY;
        tt.tm_hour += INT_HOUR;
        tt.tm_min += INT_MIN;
        mktime(&tt);
        tt.tm_isdst = NO_INFO;
    } else {
        while (tt.tm_year != year - OFFSET || tt.tm_yday <= COR_YDAY) {
            tt.tm_mday += INT_DAY;
            tt.tm_hour += INT_HOUR;
            tt.tm_min += INT_MIN;
            mktime(&tt);
            tt.tm_isdst = NO_INFO;
        }
    }
    if (tt.tm_wday == COR_WDAY) {
        tt.tm_mday += WEEK;
    } else {
        tt.tm_mday += (WEEK + 1 - tt.tm_wday) % WEEK;
    }
    mktime(&tt);
    tt.tm_isdst = NO_INFO;
    tt.tm_mday += 3 * WEEK;
    mktime(&tt);
    tt.tm_isdst = NO_INFO;
    strftime(ans, sizeof(ans), TEMP, &tt);
    printf("%s\n", ans);
}

