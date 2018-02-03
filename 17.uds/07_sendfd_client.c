#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_NAME "liucong.sendfd.server"
#define OFFSET_OF(TYPE, MEMBER) ((unsigned int)&((TYPE*)0)->MEMBER)

int main(int argc, char* argv[])
{
    int fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_NAME);
    socklen_t server_len = OFFSET_OF(struct sockaddr_un, sun_path) + strlen(SERVER_NAME) + 1;
    if (connect(fd, (struct sockaddr*)&server_addr, server_len) < 0) {
        perror("connect");
        return -1;
    }
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    struct cmsghdr chdr;
    chdr.cmsg_type = SCM_RIGHTS;
    chdr.cmsg_len = CMSG_LEN(sizeof(int));
    chdr.cmsg_level = SOL_SOCKET;
    hdr.msg_control = &chdr;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int n = recvmsg(fd, &hdr, 0);
    printf("%d bytes recved\n", n);
    if (n < 0) {
        perror("recvmsg");
        return -1;
    }
    struct cmsghdr* r_chdr = (struct cmsghdr*)(hdr.msg_control);
    int file_fd = *(int*)CMSG_DATA(r_chdr);
    printf("file_fd:%d\n", file_fd);
#define MAXLINE 4096
    lseek(file_fd, 0, SEEK_SET); // as you can see below, the current position of file pointer is at the end.!!!
    char buffer[MAXLINE];
    int count = read(file_fd, buffer, MAXLINE);
    if (n < 0) {
        perror("read");
        return -1;
    } else {
        printf("%d bytes in file\n", count);
    }
    buffer[count] = 0;
    printf("file content:%s\n", buffer);
    struct stat buffer_stat;
    fstat(file_fd, &buffer_stat);
    printf("size of file:%ld\n", buffer_stat.st_size);
    return 0;
}
