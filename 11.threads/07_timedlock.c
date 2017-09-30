#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[])
{
    struct timespec p_time;
    clock_gettime(CLOCK_REALTIME, &p_time);
    struct tm* p_tm = localtime(&p_time.tv_sec);
    char buf[100];
    strftime(buf, 100, "%r", p_tm);
    printf("current time is:%s\n", buf);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    p_time.tv_sec += 3;
    int res = pthread_mutex_timedlock(&mutex, &p_time);
    if (res == 0)
        printf("mutex locked again\n");
    else
        printf("timedlock error:%s\n", strerror(res));
    struct timespec l_time;
    clock_gettime(CLOCK_REALTIME, &l_time);
    struct tm* l_tm = localtime(&l_time.tv_sec);
    strftime(buf, 100, "%r", l_tm);
    printf("current time is:%s\n", buf);
    return 0;
}
