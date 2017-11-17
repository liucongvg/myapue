#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int lockfile(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_whence = SEEK_SET;
    return fcntl(fd, F_SETLK, &fl);
}

int main(int argc, char* argv[])
{
    int fd = open("tempfile", O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0)
        return -1;
    if (lockfile(fd) < 0)
        printf("error lockfile\n");
    else {
        printf("successfully lock file\n");
        pause();
    }
    return 0;
}
