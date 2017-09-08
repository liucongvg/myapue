#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// long long count = 0;
struct timeval end;
static void check_time(const char* str);
static void* thread_entry(void* s);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define THREAD_NUM 8

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
    int i;
    pthread_t thread_array[THREAD_NUM];
    long long sum[THREAD_NUM] = { 0 };
    for (i = 0; i < THREAD_NUM; ++i) {
        pthread_create(&thread_array[i], NULL, thread_entry, &sum[i]);
    }
    long long total = 0;
    for (i = 0; i < THREAD_NUM; ++i) {
        pthread_join(thread_array[i], NULL);
        total += sum[i];
    }
    printf("%s:%d total:%lld\n", s, getpid(), total);
    return 0;
}

static void check_time(const char* str)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec) {
        // printf("%s count:%lld\n", str, count);
        pthread_exit((void*)0);
    }
}

static void* thread_entry(void* s)
{
    // printf("enter thread_entry\n");
    while (1) {
        // pthread_mutex_lock(&mutex);
        long long* sum = (long long*)s;
        *sum += 1;
        if (*sum == 0) {
            // pthread_mutex_unlock(&mutex);
            printf("sum overflow\n");
            return NULL;
        }
        // pthread_mutex_unlock(&mutex);
        check_time(NULL);
    }
}
