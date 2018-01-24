#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define PATHNAME "/tmp/liucong_svmsg"
#define PROJ_ID 0

int main(int argc, char* argv[])
{
    if (creat(PATHNAME, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
        perror("create error");
        return -1;
    }
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok error");
        return -1;
    }
    int msg_id = msgget(key, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (msg_id < 0) {
        perror("msgget error");
        return -1;
    }
#define MSG_SIZE 1024
    struct msgbuf {
        long mtype;
        char message[MSG_SIZE];
    } msg = { 1, { 0 } };
    int n = msgrcv(msg_id, &msg, sizeof(msg.message), 0, MSG_NOERROR);
    if (n < 0) {
        perror("msgrcv error");
        return -1;
    }
    msg.message[n]=0;
    printf("server recv:%s\n",msg.message);
    char new_message[MSG_SIZE];
    snprintf(new_message, MSG_SIZE, "---%s---", msg.message);
    int length = strlen(new_message);
    strcpy(msg.message, new_message);
    if (msgsnd(msg_id, &msg, length, 0) < 0) {
        perror("msgsnd error");
        return -1;
    }
    if (msgctl(msg_id, IPC_RMID, NULL) < 0) {
        perror("msgctl error");
        return -1;
    }
    return 0;
}
