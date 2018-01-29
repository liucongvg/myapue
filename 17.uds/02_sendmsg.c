#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define KEY 0x1234
#define MAXLINE 4096
struct msgbuf {
    long mtype;
    char message[MAXLINE];
};

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("three arguments required\n");
        return -1;
    }
    char* key_offset = argv[1];
    char* message = argv[2];
    int offset = atoi(key_offset);
    int msq_id = msgget(KEY + offset, S_IRUSR | S_IWUSR);
    if (msq_id < 0) {
        perror("msgget");
        return -1;
    }
    struct msgbuf buf;
    buf.mtype = 1;
    strcpy(buf.message, message);
    printf("id:%d\n", msq_id);
    if (msgsnd(msq_id, &buf, strlen(buf.message), 0) < 0) {
        perror("msgsnd");
        return -1;
    }
    return 0;
}
