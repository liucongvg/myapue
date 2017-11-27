#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    struct aio_wrap wraps[NBUF];
    memset(wraps, 0, sizeof(wraps));
    int i;
    const struct aiocb* aio_list[NBUF];
    for (i = 0; i < BSZ; ++i) {
        wraps[i].op = UNUSED;
        wraps[i].aiocb.aio_buf = wraps[i].data;
        wraps[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
        aio_list[i] = NULL;
    }
    int offset = 0;
    int res;
    int num_to_process = 0;
    int n;
    while (1) {
        for (i = 0; i < NBUF; ++i) {
            switch (wraps[i].op) {
            case UNUSED:
                if (offset >= statbuf.st_size)
                    continue;
                wraps[i].op = READ_PENDING;
                wraps[i].aiocb.aio_fildes = in_fd;
                wraps[i].aiocb.aio_offset = offset;
                offset += BSZ;
                if (offset >= statbuf.st_size)
                    wraps[i].last = 1;
                wraps[i].aiocb.aio_nbytes = BSZ;
                if (aio_read(&wraps[i].aiocb) < 0) {
                    printf("error in aio_read\n");
                    exit(-1);
                }
                aio_list[i] = &wraps[i].aiocb;
                ++num_to_process;
                break;
            case READ_PENDING:
                if ((res = aio_error(&wraps[i].aiocb)) == EINPROGRESS)
                    continue;
                if (res != 0) {
                    printf("aio_error\n");
                    exit(-1);
                }
                if ((n = aio_return(&wraps[i].aiocb)) < 0) {
                    printf("error aio_return\n");
                    exit(-1);
                }
                if (n != BSZ && !wraps[i].last) {
                    printf("error read count\n");
                    exit(-1);
                }
                // int j;
                // for (j = 0; j < n; ++j)
                //    wraps[i].data[j] = translate(wraps[i].data[j]);
                wraps[i].op = WRITE_PENDING;
                wraps[i].aiocb.aio_fildes = out_fd;
                wraps[i].aiocb.aio_nbytes = n;
                if (aio_write(&wraps[i].aiocb) < 0) {
                    perror("aio_write");
                    exit(-1);
                }
                break;
            case WRITE_PENDING:
                if ((res = aio_error(&wraps[i].aiocb)) == EINPROGRESS)
                    continue;
                if (res != 0) {
                    printf("aio_error error\n");
                    exit(-1);
                }
                int n;
                if ((n = aio_return(&wraps[i].aiocb)) < 0) {
                    printf("aio_return error\n");
                    exit(-1);
                }
                if (n != wraps[i].aiocb.aio_nbytes) {
                    printf("short write\n");
                    exit(-1);
                }
                aio_list[i] = NULL;
                wraps[i].op = UNUSED;
                --num_to_process;
                break;
            }
        }
        if (num_to_process == 0) {
            if (offset >= statbuf.st_size)
                break;
        } else {
            if (aio_suspend(aio_list, NBUF, NULL) < 0) {
                perror("aio_suspend");
            }
        }
    }
    wraps[0].aiocb.aio_fildes = out_fd;
    aio_fsync(O_SYNC, &wraps[0].aiocb);
    exit(0);
}
