#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define OFFSET_OF(TYPE, MEMBER) ((unsigned int)(&((TYPE*)0)->MEMBER))

#define SERVER_NAME "liucong.unique_connections_server"
#define CLIENT_NAME "liucong.unique_connections_client"

int client_connect(const char* server_name, const char* client_name);

int main(int argc, char* argv[])
{
    int fd = client_connect(SERVER_NAME, CLIENT_NAME);
    if (fd < 0)
        return -1;
    char* message = "I am liucong";
    int n = send(fd, message, strlen(message), 0);
    if (n < 0) {
        perror("send");
        close(fd);
        return -1;
    }
    return 0;
}

int client_connect(const char* server_name, const char* client_name)
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_un client_addr;
    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, client_name);
    socklen_t len_client = OFFSET_OF(struct sockaddr_un, sun_path) + strlen(client_name);
    unlink(client_name);
    if (bind(fd, (struct sockaddr*)&client_addr, len_client) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, server_name);
    socklen_t len_server = OFFSET_OF(struct sockaddr_un, sun_path) + strlen(server_name);
    if (connect(fd, (struct sockaddr*)&server_addr, len_server) < 0) {
        perror("connect");
        close(fd);
        return -1;
    }
    return fd;
}
