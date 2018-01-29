#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>

#define NUMBER_THREADS 3
#define KEY 0x1234
#define MAXLINE 100
struct thread_argv {
    int queue_id;
    int fd_to_write;
} argv_array[NUMBER_THREADS];

struct msgbuf {
    long mtype;
    char message[MAXLINE];
};

void* thread_function(void* argv)
{
    struct thread_argv gv = *((struct thread_argv*)argv);
    ssize_t n;
    struct msgbuf buf;
    memset(&buf, 0, sizeof(buf));
    buf.mtype = 1;
    printf("id:%d\n", gv.queue_id);
    while (1) {
        if ((n = msgrcv(gv.queue_id, &buf, MAXLINE, 0, 0)) < 0) {
            perror("msgrcv");
            return NULL;
        }
        buf.message[n] = 0;
        if (write(gv.fd_to_write, buf.message, strlen(buf.message)) < 0) {
            perror("write");
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    int epfd = epoll_create(NUMBER_THREADS);
    if (epfd < 0) {
        perror("epoll_create");
        return -1;
    }
    int i;
    for (i = 0; i < NUMBER_THREADS; ++i) {
        int msq_id = msgget(KEY + i, IPC_CREAT | S_IRUSR | S_IWUSR);
        if (msq_id < 0) {
            perror("msgget");
            return -1;
        }
        int fd[2]; // fd[0] is used for read, and fd[1] is used for write
        if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0) {
            perror("socketpair");
            return -1;
        }
        struct epoll_event event;
        event.data.fd = fd[0];
        event.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_ADD, fd[0], &event);
        argv_array[i].queue_id = msq_id;
        argv_array[i].fd_to_write = fd[1];
        pthread_t tid;
        if (pthread_create(&tid, NULL, thread_function, &argv_array[i]) != 0) {
            printf("pthread_create error\n");
            return -1;
        }
    }
    struct epoll_event events[NUMBER_THREADS];
    while (1) {
        int n;
        if ((n = epoll_wait(epfd, events, NUMBER_THREADS, -1)) < 0) {
            perror("epoll_wait");
            continue;
        }
        printf("n:%d\n", n);
        char message[4096];
        int i;
        for (i = 0; i < n; ++i) {
            if (events[i].events & EPOLLIN) {
                n = read(events->data.fd, message, MAXLINE);
                if (n < 0) {
                    perror("read");
                    continue;
                }
                message[n] = 0;
                printf("message read:%s\n", message);
            }
        }
    }
    // don't forget to rm the queue at right time
    return 0;
}
