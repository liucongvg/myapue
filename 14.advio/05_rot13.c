#include <aio.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BSZ 4096
#define NBUF 8
enum operation { UNUSED = 0, READ_PENDING = 1, WRITE_PENDING = 2 };
struct aio_wrap {
    enum operation op;
    int last;
    struct aiocb aiocb;
    unsigned char data[BSZ];
};

int main(int argc, char* argv[])
{
    if (argc != 3)
        return -1;
    int in_fd = open(argv[1], O_RDONLY);
    if (in_fd < 0)
        return -1;
    int out_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (out_fd < 0)
        return -1;
    struct stat statbuf;
    fstat(in_fd, &statbuf);
    struct aio_wrap wraps[BSZ];

}
