#include <pthread.h>
#include <stdio.h>

static void* f_thread1(void* arg);
static void* f_thread2(void* arg);

int main(int argc, char* argv[])
{
    pthread_t tid1, tid2;
    void *res1, *res2;
    pthread_create(&tid1, NULL, f_thread1, NULL);
    pthread_join(tid1, &res1);
    pthread_create(&tid2, NULL, f_thread2, NULL);
    pthread_join(tid2, &res2);
    printf("thread1 res:%ld\n", (long)res1);
    printf("thread2 res:%ld\n", (long)res2);
    return 0;
}

static void* f_thread1(void* arg)
{
    // pthread_exit((void*)1);
    return (void*)1;
}

static void* f_thread2(void* arg) { pthread_exit((void*)2); }
