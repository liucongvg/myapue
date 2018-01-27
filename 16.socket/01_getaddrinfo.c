#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void print_family(struct addrinfo* ai);
void print_type(struct addrinfo* ai);
void print_protocal(struct addrinfo* ai);
void print_flags(struct addrinfo* ai);

int main(int argc, char* argv[])
{
    if (argc != 3 && argc != 2)
        return -1;
    char* node = argv[1];
    char* service;
    if (argc == 2)
        service = NULL;
    else
        service = argv[2];
    struct addrinfo hint;
    struct addrinfo* res_list;
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    int res;
    if ((res = getaddrinfo(node, service, &hint, &res_list)) != 0) {
        printf("getaddrinfo error:%s\n", gai_strerror(res));
        return -1;
    }
    struct addrinfo* current_info;
    char addr[INET_ADDRSTRLEN];
    for (current_info = res_list; current_info != NULL; current_info = current_info->ai_next) {
        print_flags(current_info);
        print_family(current_info);
        print_type(current_info);
        print_protocal(current_info);
        printf("host:%s\n", current_info->ai_canonname ? current_info->ai_canonname : "canonical name NULL");
        if (current_info->ai_family == AF_INET)
            ;
        {
            struct sockaddr_in* iaddr = (struct sockaddr_in*)current_info->ai_addr;
            const char* res = inet_ntop(AF_INET, &iaddr->sin_addr, addr, INET_ADDRSTRLEN);
            if (res) {
                printf("address:%s\n", addr);
            } else {
                perror("inet_ntop");
            }
            printf("port:%d\n", ntohs(iaddr->sin_port));
        }
        printf("----------------------------\n");
    }
}

void print_family(struct addrinfo* ai)
{
    printf("family:");
    switch (ai->ai_family) {
    case AF_INET:
        printf("inet");
        break;
    case AF_INET6:
        printf("inet6");
        break;
    case AF_UNIX:
        printf("unix");
        break;
    case AF_UNSPEC:
        printf("unspecified");
        break;
    default:
        printf("unknown");
        break;
    }
    printf("\n");
}

void print_type(struct addrinfo* ai)
{
    printf("type:");
    switch (ai->ai_socktype) {
    case SOCK_STREAM:
        printf("stream");
        break;
    case SOCK_DGRAM:
        printf("datagram");
        break;
    case SOCK_SEQPACKET:
        printf("seqpacket");
        break;
    case SOCK_RAW:
        printf("raw");
        break;
    default:
        printf("unknown");
        break;
    }
    printf("\n");
}

void print_protocal(struct addrinfo* ai)
{
    printf("protocal:");
    switch (ai->ai_protocol) {
    case 0:
        printf("default");
        break;
    case IPPROTO_TCP:
        printf("TCP");
        break;
    case IPPROTO_UDP:
        printf("UDP");
        break;
    case IPPROTO_RAW:
        printf("raw");
        break;
    default:
        printf("unknown");
        break;
    }
    printf("\n");
}

void print_flags(struct addrinfo* ai)
{
    printf("flasg:");
    if (ai->ai_flags == 0) {
        printf("0");
    } else {
        if (ai->ai_flags & AI_PASSIVE)
            printf("passive");
        if (ai->ai_flags & AI_CANONNAME)
            printf("canonical name");
        if (ai->ai_flags & AI_NUMERICHOST)
            printf("numeric host");
        if (ai->ai_flags & AI_NUMERICSERV)
            printf("numeric service");
        if (ai->ai_flags & AI_ADDRCONFIG)
            printf("config"); // don't understand
        if (ai->ai_flags & AI_V4MAPPED)
            printf("v4mapped");
        if (ai->ai_flags & AI_ALL)
            printf("all");
    }
    printf("\n");
}
