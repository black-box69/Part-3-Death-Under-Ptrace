#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

//made by: CDECL

enum
{
    BASE = 10
};

struct Elem
{
    struct Elem *next;
    char *str;
};

int
strtol_safe(struct Elem *cur)
{
    errno = 0;
    char *endptr = NULL;
    long long int digit = strtol(cur->str, &endptr, BASE);
    if (errno || endptr == cur->str || *endptr || digit >= INT_MAX || digit < INT_MIN) {
        return 0;
    }
    return 1;
}

int
read_str_field(struct Elem *cur)
{
    char *endptr = NULL;
    return strtol(cur->str, &endptr, BASE);
}

struct Elem *
create_elem(struct Elem *cur)
{
    errno = 0;
    struct Elem *new_elem = calloc(1, sizeof(*new_elem));
    if (new_elem == NULL) {
        exit(1);
    }
    int digit = read_str_field(cur);
    char *buffer = NULL;
    asprintf(&buffer, "%d", digit + 1);
    new_elem->next = cur;
    new_elem->str = strdup(buffer);
    free(buffer);
    return new_elem;
}

struct Elem *
dup_elem(struct Elem *head)
{
    if (head == NULL) {
        return head;
    }
    struct Elem *ans = head;
    if (strtol_safe(head)) {
        struct Elem *new_elem = create_elem(head);
        ans = new_elem;
    }
    while (head->next != NULL) {
        if (strtol_safe(head->next)) {
            struct Elem *new_elem = create_elem(head->next);
            head->next = new_elem;
            head = new_elem->next;
        } else {
            head = head->next;
        }
    }
    return ans;
}

