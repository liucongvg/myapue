#include <pthread.h>
#include <stdio.h>

void cleanup(void*);
void* thread1_func(void*);
void* thread2_func(void*);

int main(int argc, char* argv[])
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_join(thread1, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);
    pthread_join(thread2, NULL);
    return 0;
}

void cleanup(void* arg) { printf("%s\n", (char*)arg); }

void* thread1_func(void* arg)
{
    pthread_cleanup_push(cleanup, "thread1_func push1");
    pthread_cleanup_push(cleanup, "thread1_func push2");
    pthread_exit(NULL);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return NULL;
}

void* thread2_func(void* arg)
{
    pthread_cleanup_push(cleanup, "thread2_func push1");
    pthread_cleanup_push(cleanup, "thread2_func push2");
    return NULL;
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
}
