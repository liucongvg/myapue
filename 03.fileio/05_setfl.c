#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int set_fl(int fd, int flags)
{
    int res;
    if ((res = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl error");
        return -1;
    }
    res |= flags;
    if (fcntl(fd, F_SETFL, res) < 0) {
        perror("fcntl error");
        return -1;
    }
    return 0;
}
int main(int argc, char* argv[]) { return 0; }
