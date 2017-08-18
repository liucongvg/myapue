#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (open("tempfile", O_RDWR) < 0) {
        perror("open error");
        return -1;
    }
    if (unlink("tempfile") < 0) {
        perror("unlink error");
        return -1;
    }
    sleep(20);
    printf("done!\n");
    return 0;
}
