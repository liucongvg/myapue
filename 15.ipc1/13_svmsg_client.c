#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define PATHNAME "/tmp/liucong_svmsg"
#define PROJ_ID 2

int main(int argc, char* argv[])
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    int msg_id = msgget(key, 0);
    if (msg_id < 0)
        return -1;
#define MSG_SIZE 1024
    struct msgbuf {
        long mtype;
        char message[MSG_SIZE];
    } msg = { 1, "I am liucong" };
    if (msgsnd(msg_id, &msg, strlen(msg.message), 0) < 0)
        return -1;
    int n = msgrcv(msg_id, &msg, MSG_SIZE, 0, 0);
    if (n < 0)
        return -1;
    msg.message[n] = 0;
    printf("client recv:%s\n", msg.message);
    msgctl(msg_id, IPC_RMID, NULL);
    return 0;
}
