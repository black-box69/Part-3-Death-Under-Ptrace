#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

//made by: ded

enum
{
    MY_MAX_INT = (signed) (~(unsigned) 0 >> 1),
    STRING_LENGTH = 12
};

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *
create_new_node(int val)
{
    struct Elem *tmp = malloc(sizeof(struct Elem));
    char *str = malloc(STRING_LENGTH * sizeof(char));
    snprintf(str, STRING_LENGTH, "%d", val);
    tmp->str = str;
    return tmp;
}

struct Elem *
dup_elem(struct Elem *head)
{
    if (head == NULL) {
        return head;
    }
    char *eptr = NULL;
    errno = 0;

    struct Elem *back_iterator = head;
    struct Elem *front_iterator = back_iterator->next;

    long lval = strtol(head->str, &eptr, 10);
    if (!(errno || *eptr || eptr == head->str || (int) lval != lval) && lval < MY_MAX_INT) {
        struct Elem *tmp_elem = create_new_node(lval + 1);
        tmp_elem->next = head;
        head = tmp_elem;
    }

    while (front_iterator != NULL) {
        eptr = NULL;
        errno = 0;
        lval = strtol(front_iterator->str, &eptr, 10);
        if (!(errno || *eptr || eptr == front_iterator->str || (int) lval != lval) && lval < MY_MAX_INT) {
            struct Elem *tmp_elem = create_new_node(lval + 1);
            tmp_elem->next = front_iterator;
            back_iterator->next = tmp_elem;
            back_iterator = tmp_elem;
        }
        back_iterator = back_iterator->next;
        front_iterator = front_iterator->next;
    }
    return head;
}
