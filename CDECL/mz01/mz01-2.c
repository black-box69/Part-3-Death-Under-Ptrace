#include <stdio.h>
#include <stdlib.h>

//made by: CDECL

enum
{
    START = 1,
    LOWERCASE = 11,
    UPPERCASE = 37,
    ZERO_SYM = 0,
    END_SYM = 63,
    SECONDBIT = 251,
    THIRDBIT = 8
};

unsigned char
encoder(unsigned char sym)
{
    if ('A' <= sym && sym <= 'Z') {
        return sym - 'A' + UPPERCASE;
    } else if ('a' <= sym && sym <= 'z') {
        return sym - 'a' + LOWERCASE;
    } else if ('0' <= sym && sym <= '9') {
        return sym - '0' + START;
    } else {
        return 0;
    }
}

unsigned char
decoder(unsigned char sym)
{
    if (sym == ZERO_SYM) {
        return '@';
    } else if (sym == END_SYM) {
        return '#';
    } else if (START <= sym && sym < LOWERCASE) {
        return sym - START + '0';
    } else if (LOWERCASE <= sym && sym < UPPERCASE) {
        return sym - LOWERCASE + 'a';
    } else if (UPPERCASE <= sym && sym < END_SYM) {
        return sym - UPPERCASE + 'A';
    } else {
        exit(1);
    }
}

int
main(void)
{
    int cur;
    while ((cur = getchar_unlocked()) != EOF) {
        unsigned char coded = encoder((unsigned char) cur);
        if (coded != 0) {
            coded &= SECONDBIT;
            coded ^= THIRDBIT;
            putchar_unlocked(decoder(coded));
        }
    }
    printf("\n");
    return 0;
}

