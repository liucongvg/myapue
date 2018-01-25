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
    struct mq_attr attr;
    attr.mq_msgsize = 1024;
    attr.mq_maxmsg = 3;
    mqd_t mqd = mq_open(PATHANME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    if (mqd == (mqd_t)-1) {
        perror("mq_open");
        return -1;
    }
#define MAXLINE 2048
    char line[MAXLINE];
    size_t n = mq_receive(mqd, line, MAXLINE, 0);
    if (n == -1) {
        perror("mq_receive");
        return -1;
    }
    char new_line[MAXLINE];
    snprintf(new_line, MAXLINE, "---%s---", line);
    mq_send(mqd, new_line, strlen(new_line), 0);
    mq_close(mqd);
    sleep(1); // for client to read message
    mq_unlink(PATHANME);
    return 0;
}
