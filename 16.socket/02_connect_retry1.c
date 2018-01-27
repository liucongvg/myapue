#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXSLEEP 128

int connet_retry(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
    int cur_sec;
    for (cur_sec = 1; cur_sec <= MAXSLEEP; cur_sec <<= 1) {
        if (connect(sockfd, addr, addrlen) == 0)
            return 0;
        if (cur_sec <= MAXSLEEP / 2)
            sleep(cur_sec);
    }
    return -1;
}

int main(int argc, char* argv[]) { return 0; }
