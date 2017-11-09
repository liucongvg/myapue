#include <pthread.h>
#include <signal.h>
#include <stdio.h>

static sigset_t mask;
static int flag;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

static void* thread_handler(void* argv)
{
    int signo;
    while (1) {
        if (sigwait(&mask, &signo))
            return NULL;
        switch (signo) {
        case SIGINT:
            printf("interrupt\n");
            break;
        case SIGQUIT:
            pthread_mutex_lock(&mutex);
            flag = 1;
            pthread_mutex_unlock(&mutex);
            printf("tell main to quit\n");
            pthread_cond_signal(&condition);
            return 0;
        default:
            printf("unexpected signal\n");
            return 0;
        }
    }
}

int main(int argc, char* argv[])
{
    sigset_t oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    if (pthread_sigmask(SIG_BLOCK, &mask, &oldmask))
        return -1;
    pthread_t tid;
    pthread_create(&tid, NULL, thread_handler, NULL);
    pthread_mutex_lock(&mutex);
    while (!flag)
        pthread_cond_wait(&condition, &mutex);
    pthread_mutex_unlock(&mutex);
    if (pthread_sigmask(SIG_SETMASK, &oldmask, NULL)) {
        printf("error set mask\n");
        return -1;
    }
    return 0;
}
