#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int make_thread(void*(thread_function)(void*), void* arg)
{
    int res;
    pthread_t tid;
    pthread_attr_t attr;
    if ((res = pthread_attr_init(&attr)))
        return res;
    res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (!res)
        res = pthread_create(&tid, &attr, thread_function, arg);
    pthread_attr_destroy(&attr);
    return res;
}

void* thread_function(void* argv)
{
    printf("thread_function invoked\n");
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_function, NULL);
    /*
    if (make_thread(thread_function, NULL))
        printf("make_thread error\n");
    */
    pause();
    return 0;
}
