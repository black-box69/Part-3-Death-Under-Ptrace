#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//made by: ded

int
power(int a, int b)
{
    unsigned int res = (unsigned int) a * (unsigned int) b;
    res &= 0xffffffff;
    return (int) res;
}

int
summarize(int a, int b)
{
    unsigned int res = (unsigned int) a + (unsigned int) b;
    res &= 0xffffffff;
    return (int) res;
}

int (*func)(int, int) = summarize;

void
hndlr_INT(int sig)
{
    signal(sig, hndlr_INT);
    func = summarize;
}

void
hndlr_QUIT(int sig)
{
    signal(sig, hndlr_QUIT);
    func = power;
}

int
main()
{
    signal(SIGINT, hndlr_INT);
    signal(SIGQUIT, hndlr_QUIT);
    setbuf(stdin, NULL);
    int res = 0;

    printf("%d\n", getpid());
    fflush(stdout);

    int tmp = 0;
    while (scanf("%d", &tmp) != EOF) {
        res = func(res, tmp);
        printf("%d\n", res);
        fflush(stdout);
    }
    return 0;
}
