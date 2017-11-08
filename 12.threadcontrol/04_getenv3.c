#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENV_VALUE_SIZE 4096
extern char** environ;
static pthread_key_t key;
static pthread_once_t once_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void thread_init(void) { pthread_key_create(&key, free); }

char* getenv(const char* name)
{
    pthread_once(&once_init, thread_init);
    pthread_mutex_lock(&mutex);
    char* value_buffer = pthread_getspecific(key);
    if (!value_buffer) {
        if (!(value_buffer = malloc(MAX_ENV_VALUE_SIZE))) {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        pthread_setspecific(key, value_buffer);
    }
    int len = strlen(name);
    int i;
    for (i = 0; environ[i] != NULL; ++i) {
        if (!strncmp(name, environ[i], len) && (environ[i][len] == '=')) {
            strncpy(value_buffer, &environ[i][len + 1], MAX_ENV_VALUE_SIZE - 1);
            pthread_mutex_unlock(&mutex);
            return value_buffer;
        }
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char* argv[])
{
    printf("HOME:%s\n", getenv("HOME"));
    return 0;
}
