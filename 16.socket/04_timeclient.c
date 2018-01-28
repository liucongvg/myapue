#include <limits.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
        return -1;
    int type;
    if (!strcmp(argv[1], "tcp"))
        type = SOCK_STREAM;
    else if (!strcmp(argv[1], "udp"))
        type = SOCK_DGRAM;
    else {
        return -1;
    }
    char host[HOST_NAME_MAX];
    gethostname(host, HOST_NAME_MAX);
    char* service = "my_service_test";
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype = type;
    struct addrinfo* res_list;
    int res;
    if ((res = getaddrinfo(host, service, &hint, &res_list)) != 0) {
        printf("getaddrinfo:%s\n", gai_strerror(res));
        return -1;
    }
    int fd = socket(AF_INET, type, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    if (connect(fd, res_list->ai_addr, res_list->ai_addrlen) < 0) {
        perror("connect");
        close(fd);
        return -1;
    }
#define MAXLINE 4096
    char buf[MAXLINE];
    if (type == SOCK_DGRAM) {
        if (send(fd, NULL, 0, 0) < 0) {
            perror("send");
            close(fd);
            return -1;
        }
    }
    ssize_t n = recv(fd, buf, MAXLINE, 0);
    if (n < 0) {
        perror("recv");
        close(fd);
        return -1;
    } else if (n == 0) {
        close(fd);
    } else {
        buf[n] = 0;
        printf("recv:%s\n", buf);
    }
    close(fd);
    return 0;
}
