#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

#define process_limit(name) print_limit(#name, name)
static void print_limit(char* name, int resource);

int main(int argc, char* argv[])
{
    process_limit(RLIMIT_AS);
    process_limit(RLIMIT_CORE);
    process_limit(RLIMIT_CPU);
    process_limit(RLIMIT_DATA);
    process_limit(RLIMIT_FSIZE);
    process_limit(RLIMIT_MEMLOCK);
    process_limit(RLIMIT_MSGQUEUE);
    process_limit(RLIMIT_NICE);
    process_limit(RLIMIT_NOFILE);
    process_limit(RLIMIT_NPROC);
    // process_limit(RLIMIT_RSS);
    process_limit(RLIMIT_RTPRIO);
    process_limit(RLIMIT_RTTIME);
    process_limit(RLIMIT_SIGPENDING);
    process_limit(RLIMIT_STACK);
    return 0;
}

static void print_limit(char* name, int resource)
{
    struct rlimit limit;
    unsigned long long value;
    if (getrlimit(resource, &limit) < 0) {
        perror("getrlimit error");
        exit(EXIT_FAILURE);
    }
    printf("%-14s  ", name);
    if (limit.rlim_cur == RLIM_INFINITY) {
        printf("(infinite)  ");
    } else {
        value = limit.rlim_cur;
        printf("%10lld  ", value);
    }
    if (limit.rlim_max == RLIM_INFINITY) {
        printf("(infinite)");
    } else {
        value = limit.rlim_max;
        printf("%10lld", value);
    }
    putchar('\n');
}
