#include <arpa/inet.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
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
    else
        return -1;
    int fd = socket(AF_INET, type, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    char host[HOST_NAME_MAX];
    if (gethostname(host, HOST_NAME_MAX) < 0) {
        perror("gethostname");
        close(fd);
        return -1;
    }
    char* service = "my_service_test";
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype = type;
    struct addrinfo* res_list;
    int res;
    if ((res = getaddrinfo(host, service, &hint, &res_list)) != 0) {
        printf("getaddrinfo:%s\n", gai_strerror(res));
        close(fd);
        return -1;
    }
    if (bind(fd, res_list->ai_addr, res_list->ai_addrlen) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }
#define MAXLINE 4096
    char return_buf[MAXLINE];
    if (type == SOCK_STREAM) {
        if (listen(fd, 10) < 0) {
            perror("listen");
            close(fd);
            return -1;
        }
        struct sockaddr addr;
        socklen_t len = sizeof(struct sockaddr_in); // you must initialize len, not just socklen_t len;!!!
        int new_fd = accept(fd, &addr, &len);
        if (new_fd < 0) {
            perror("accept");
            close(fd);
            return -1;
        }
        FILE* stream = popen("/usr/bin/uptime", "r");
        fgets(return_buf, MAXLINE, stream);
        if (send(new_fd, return_buf, strlen(return_buf), 0) < 0) {
            perror("send");
            close(new_fd);
            close(fd);
            return -1;
        }
    } else {
        char tempbuf[100] = { 0 };
        struct sockaddr addr;
        socklen_t len
            = sizeof(struct sockaddr_in); // NOTE: you must initialize the len value!!!! not just socklen_t len;
        ssize_t n = recvfrom(fd, tempbuf, 100, 0, &addr, &len);
        printf("host:%s\n", inet_ntoa(((struct sockaddr_in*)&addr)->sin_addr));
        printf("host:%d\n", ntohs(((struct sockaddr_in*)&addr)->sin_port));
        tempbuf[n] = 0;
        // printf("recv form client:%s\n", tempbuf);
        FILE* stream = popen("/usr/bin/uptime", "r");
        fgets(return_buf, MAXLINE, stream);
        if (sendto(fd, return_buf, strlen(return_buf), 0, &addr, len) < 0) {
            perror("sendto");
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}
