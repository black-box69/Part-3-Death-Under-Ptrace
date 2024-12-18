#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

//made by: ded

enum
{
    OCT_BASE = 8,
    BINARY_NUM_SIZE = 9,
    MASK = 256
};

const char pattern[] = "rwxrwxrwx";

int
base8to10(int value)
{
    int result = 0;
    int base = 1;
    while (value > 0) {
        result += (value % 10) * base;
        base *= OCT_BASE;
        value /= 10;
    }
    return result;
}

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        int input_val = atoi(argv[i]);
        int dec_input_val = base8to10(input_val);
        int tmp_mask = MASK;
        for (int j = 0; j < 9; j++) {
            if (dec_input_val & tmp_mask) {
                printf("%c", pattern[j]);
            } else {
                printf("%c", '-');
            }
            tmp_mask = tmp_mask >> 1;
        }
        printf("\n");
    }
    return 0;
}
