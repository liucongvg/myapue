#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    int res;
    if (argc != 2) {
        printf("Two arguments required!\n");
        exit(EXIT_FAILURE);
    }
    if ((res = fcntl(atoi(argv[1]), F_GETFL)) < 0) {
        perror("fcntl error");
        exit(EXIT_FAILURE);
    }
    switch (res & O_ACCMODE) {
    case O_RDONLY:
        printf("read only");
        break;
    case O_WRONLY:
        printf("write only");
        break;
    case O_RDWR:
        printf("read write");
        break;
    default:
        printf("unknown access mode");
        break;
    }
    if (res & O_APPEND)
        printf(", append");
    if (res & O_NONBLOCK)
        printf(", nonblocking");
    if (res & O_SYNC)
        printf(", synchronous writes");
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (res & O_FSYNC)
        printf("synchronous writes");
#endif
    putchar('\n');
    return 0;
}
