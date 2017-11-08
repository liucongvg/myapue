#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

static pthread_mutex_t mutex;
static pthread_once_t init = PTHREAD_ONCE_INIT;
extern char** environ;

static void thread_init(void)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

int getenv_r(const char* name, char* buffer, int size)
{
    pthread_once(&init, thread_init);
    int len = strlen(name);
    pthread_mutex_lock(&mutex);
    int i;
    for (i = 0; environ[i] != NULL; ++i) {
        if (!strncmp(name, environ[i], len) && (environ[i][len] == '=')) {
            int value_len = strlen(&environ[i][len + 1]);
            if (value_len >= size) {
                pthread_mutex_unlock(&mutex);
                return ENOSPC;
            }
            strcpy(buffer, &environ[i][len + 1]);
            pthread_mutex_unlock(&mutex);
            return 0;
        }
    }
    pthread_mutex_unlock(&mutex);
    return ENOENT;
}

int main(int argc, char* argv[])
{
    char env[1024];
    getenv_r("HOME", env, 1024);
    printf("HOME:%s\n", env);
    return 0;
}
