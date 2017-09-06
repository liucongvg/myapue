#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

long long count;
struct timeval end;
static void check_time(const char* str);

int main(int argc, char* argv[])
{
    pid_t pid;
    char* s;
    int nzero, ret;
    int adj = 0;
    setbuf(stdout, NULL);
#if defined(NZERO)
    nzero = NZERO;
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif
    printf("NZERO = %d\n", nzero);
    if (argc == 2)
        adj = strtol(argv[1], NULL, 10);
    gettimeofday(&end, NULL);
    end.tv_sec += 10;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        s = "child";
        printf("child nice:%d, adjusted by %d\n", nice(0) + nzero, adj);
        errno = 0;
        if ((ret = nice(adj)) == -1 && errno != 0) {
            perror("nice error");
            return -1;
        }
        printf("child now nice:%d\n", ret + nzero);
    } else {
        s = "parent";
        printf("parent nice:%d\n", nice(0) + nzero);
    }
    while (1) {
        if (++count == 0) {
            printf("count overflow\n");
            return -1;
        }
        check_time(s);
    }
    return 0;
}

static void check_time(const char* str)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec) {
        printf("%s count:%lld\n", str, count);
        exit(0);
    }
}
