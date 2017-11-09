#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_function(void* argv)
{
    printf("new thread...\n");
    pause();
    printf("new thread ended\n");
    return NULL;
}

void prepare(void)
{
    printf("prepare...\n");
    if (pthread_mutex_lock(&mutex1))
        printf("error lock mutex1\n");
    if (pthread_mutex_lock(&mutex2))
        printf("error lock mutex2\n");
}

void parent(void)
{
    printf("parent...\n");
    if (pthread_mutex_unlock(&mutex1))
        printf("error unlock mutex1\n");
    if (pthread_mutex_unlock(&mutex2))
        printf("error unlock mutex2\n");
}

void child(void)
{
    printf("child...\n");
    if (pthread_mutex_unlock(&mutex1))
        printf("error unlock mutex1\n");
    if (pthread_mutex_unlock(&mutex2))
        printf("error unlock mutex2\n");
}

int main(int argc, char* argv[])
{
    if (pthread_atfork(prepare, parent, child)) {
        return -1;
    }
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread_function, NULL)) {
        return -1;
    }
    sleep(2);
    pid_t pid;
    if ((pid = fork()) < 0) {
        printf("error fork\n");
        return -1;
    } else if (pid == 0) {
        printf("child process...\n");
    } else {
        printf("parent process...\n");
    }
    return 0;
}
