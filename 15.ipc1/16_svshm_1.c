#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define PATHNAME "/tmp/liucong_svshm"
#define PROJ_ID 2
#define SIZE 4096

int main(int argc, char* argv[])
{
    if (creat(PATHNAME, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
        perror("creat");
        return -1;
    }
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok");
        return -1;
    }
    int shm_id = shmget(key, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id < 0) {
        perror("shmget");
        return -1;
    }
    void* addr = shmat(shm_id, NULL, 0);
    if (*((long*)addr) == -1) {
        perror("shmat");
        return -1;
    }
    char* message = (char*)addr;
    strcpy(message, "I am liucong");
    sleep(10); // to give another program time to get message
    if (shmdt(addr) < 0) {
        perror("shmdt");
        return -1;
    }
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
