#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define OFFSET_OF(TYPE, MEMBER) ((unsigned int)&(((TYPE*)0)->MEMBER))
#define SOCK_NAME "liucong.sock" // could be a relative path and a absolute path

int main(int argc, char* argv[])
{
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_NAME);
    unsigned int len = OFFSET_OF(struct sockaddr_un, sun_path) + strlen(addr.sun_path);
    printf("sizeof addr.sun_path:%d\n", len);
    if (bind(fd, (struct sockaddr*)&addr, len) < 0) {
        perror("bind");
        return -1;
    }
    printf("bind to AF_UNIX of liucong.socket successfully\n");
    return 0;
}
