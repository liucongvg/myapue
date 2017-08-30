#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

static char* path_alloc(size_t* size);

int main(int argc, char* argv[])
{
    char* ptr;
    size_t size;
    if (chdir("dir2") < 0) {
        perror("chdir error");
        return -1;
    }
    ptr = path_alloc(&size);
    if (getcwd(ptr, size) < 0) {
        perror("getcwd error");
        return -1;
    }
    printf("current dir:%s\n", ptr);
    return 0;
}

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;
#define PATH_MAX_GUESS 1024


static char* path_alloc(size_t* sizep)
{
    char* ptr;
    size_t size;
    if (posix_version == 0)
        posix_version = sysconf(_SC_VERSION);
    if (xsi_version == 0)
        xsi_version = sysconf(_SC_XOPEN_VERSION);
    if (pathmax == 0) {
        errno = 0;
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;
            else {
                perror("pathconf error");
                exit(EXIT_FAILURE);
            }
        } else
            ++pathmax;
    }
    if ((posix_version < 200112L) && (xsi_version < 4))
        size = pathmax + 1;
    else
        size = pathmax;
    if ((ptr = malloc(size)) == NULL) {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }
    if (sizep != NULL)
        *sizep = size;
    return ptr;
}

