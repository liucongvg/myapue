#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define OFFSET_OF(TYPE, MEMBER) ((unsigned int)&((TYPE*)0)->MEMBER)
#define BACKLOG 3
#define SERVER_NAME "liucong.sendfd.server"
#define FILE_NAME "sendfd_file"

int main(int argc, char* argv[])
{
    int fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    unlink(SERVER_NAME);
    struct sockaddr_un server_addr;
    socklen_t len = OFFSET_OF(struct sockaddr_un, sun_path) + strlen(SERVER_NAME) + 1;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_NAME);
    if (bind(fd, (struct sockaddr*)&server_addr, len) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(fd, BACKLOG) < 0) {
        perror("listen");
        return -1;
    }
    struct sockaddr_un clien_addr;
    socklen_t client_len = sizeof(client_len);
    int client_fd = accept(fd, (struct sockaddr*)&clien_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        return -1;
    }
    int file_fd = open(FILE_NAME, O_RDONLY);
    if (file_fd < 0) {
        perror("open");
        return -1;
    }
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    struct iovec msg;
    // msg.iov_len = 0;
    // msg.iov_base = NULL;
    struct cmsghdr chdr;
    chdr.cmsg_type = SCM_RIGHTS;
    chdr.cmsg_level = SOL_SOCKET;
    chdr.cmsg_len = CMSG_LEN(sizeof(int));
    *(int*)CMSG_DATA(&chdr) = file_fd;
    hdr.msg_control = &chdr;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    //hdr.msg_controllen = CMSG_SPACE(sizeof(int));
    hdr.msg_iov = &msg;
    hdr.msg_iovlen = 0;
    hdr.msg_name = NULL;
    hdr.msg_namelen = 0;
    int n;
#define MAXLINE 4096
    char buffer[MAXLINE];
    int count = read(file_fd, buffer, MAXLINE);
    if (count < 0) {
        perror("read");
        return -1;
    } else {
        printf("%d bytes in file\n", count);
    }
    buffer[count] = 0;
    printf("file content:%s\n", buffer);

    if ((n = sendmsg(client_fd, &hdr, 0)) < 0) {
        perror("sendmsg");
        return -1;
    } else {
        printf("%d bytes send\n", n);
    }
    return 0;
}
