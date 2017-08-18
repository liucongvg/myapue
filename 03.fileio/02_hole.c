#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int fd;
    char buf1[] = "1234567890";
    char buf2[] = "abcdefghij";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    if ((fd = creat("file.hole", mode)) < 0) {
        perror("create error");
        return -1;
    }
    if (write(fd, buf1, 10) != 10)
        perror("write buf1 error");
    if (lseek(fd, 16384, SEEK_CUR) < 0) {
        perror("lseek error");
        return -1;
    }
    if (write(fd, buf2, 10) != 10) {
        perror("write error");
    }
    return 0;
}
