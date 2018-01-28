#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int init_server(int type, const struct sockaddr* addr, socklen_t addr_len, int queue_len)
{ //在Linux中backlog表示已完成(ESTABLISHED)且未accept的队列大小
    int fd = socket(addr->sa_family, type, 0);
    int tmp_errno;
    if (fd < 0) {
        return -1;
    }
    if (bind(fd, addr, addr_len) < 0) {
        goto error;
    }
    if ((type == SOCK_STREAM || type == SOCK_SEQPACKET) && listen(fd, queue_len) < 0) {
        goto error;
    }
    return fd;
error:
    tmp_errno = errno;
    close(fd);
    errno = tmp_errno;
    return -1;
}

int main(int argc, char* argv[]) { return 0; }
