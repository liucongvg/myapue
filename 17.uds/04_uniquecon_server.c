#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define OFFSET_OF(TYPE, MEMBER) ((unsigned int)(&((TYPE*)0)->MEMBER))

#define SERVER_NAME "liucong.unique_connections_server"
#define MAXLINE 4096
#define ESTABLISHED_NOT_ACCEPT_QUEUE 3

int server_listen(const char* server_name);
int server_accept(int fd);

int main(int argc, char* argv[])
{
    int fd = server_listen(SERVER_NAME);
    if (fd < 0)
        return -1;
    while (1) {
        int unique_fd = server_accept(fd); // remember accept should be inside of the while loop, not outside.
        if (unique_fd < 0)
            return -1;
        char buffer[MAXLINE];
        ssize_t n = recv(unique_fd, buffer, MAXLINE, 0);
        if (n < 0) {
            perror("recv");
            continue;
        }
        buffer[n] = 0;
        printf("server recved:%s\n", buffer);
    }
}

int server_listen(const char* server_name)
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    unlink(server_name);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, server_name);
    unsigned int len = OFFSET_OF(struct sockaddr_un, sun_path) + strlen(server_name);
    printf("len:%u\n", len);
    if (bind(fd, (struct sockaddr*)&addr, len) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }
    if (listen(fd, ESTABLISHED_NOT_ACCEPT_QUEUE) < 0) {
        perror("listen");
        close(fd);
        return -1;
    }
    return fd;
}

int server_accept(int fd)
{
    struct sockaddr_un addr;
    socklen_t len = sizeof(addr);
    int new_fd = accept(fd, (struct sockaddr*)&addr, &len);
    if (new_fd < 0) {
        perror("accept");
        close(fd);
        return -1;
    }
    return new_fd;
}
