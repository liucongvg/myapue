#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void* thread_function(void*);
static void printids(const char* str);
static pthread_t tid;

int main(int argc, char* argv[])
{
    if (pthread_create(&tid, NULL, thread_function, NULL) < 0) {
        return -1;
    }
    printids("main thread:");
    sleep(1);
    return 0;
}

static void* thread_function(void* arg)
{
    printids("new thread:");
    return NULL;
}

static void printids(const char* str)
{
    printf("%s pid:%d,tid:%lu,tid_hex:0x%lx\n", str, getpid(), pthread_self(), pthread_self());
}
