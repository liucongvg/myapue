#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATHANME "/liucon_gposixmq"
// server

int main(int argc, char* argv[])
{
    mqd_t mqd = mq_open(PATHANME, O_RDWR);
    if (mqd == (mqd_t)-1) {
        perror("mq_open");
        return -1;
    }
    char* message = "I am liucong";
    mq_send(mqd, message, strlen(message), 0);
#define MAXLINE 2048
    char line[MAXLINE];
    size_t n = mq_receive(mqd, line, MAXLINE, 0);
    if (n == -1) {
        perror("mq_receive");
        return -1;
    }
    line[n] = '\n';
    line[n + 1] = 0;
    printf(line, MAXLINE, "%s\n", line);
    mq_close(mqd);
    return 0;
}
