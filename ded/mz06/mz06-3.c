#include <stdio.h>

//made by: ded

struct s1
{
    char f1;
    long long f2;
    char f3;
};

struct s2
{
    long long f2;
    char f1;
    char f3;
};

size_t
compactify(void *ptr, size_t size)
{
    size_t s1_size = sizeof(struct s1);
    size_t s2_size = sizeof(struct s2);
    size_t N = size / s1_size;
    if (N == 0) {
        return 0;
    }

    char *data = (char *) ptr;

    struct s1 s1_elem;
    struct s2 *s2_ptr;
    for (int i = 0; i < N; i++) {
        s1_elem = *(struct s1 *) (data + i * s1_size);
        s2_ptr = (struct s2 *) (data + i * s2_size);

        s2_ptr->f1 = s1_elem.f1;
        s2_ptr->f2 = s1_elem.f2;
        s2_ptr->f3 = s1_elem.f3;
    }
    return s2_size * N;
}
