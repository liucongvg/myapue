#include <pthread.h>
#include <errno.h>
#include <string.h>

static pthread_mutex_t mutex;
static pthread_once_t init = PTHREAD_ONCE_INIT;
extern char **environ;

int getenv_r(const char *name, char *buffer, int size){

}
