#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PATHNAME "/tmp/liucong_svsem"
#define PROJ_ID 2

// this program run first and sleep 1 second, then let another program run
int main(int argc, char* argv[])
{
    if (creat(PATHNAME, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
        perror("creat");
        return -1;
    }
    key_t key = ftok(PATHNAME, PROJ_ID);
    int sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (sem_id < 0) {
        perror("semget");
        return -1;
    }
    // initialize semaphore to 1
    struct sembuf sop;
    sop.sem_op = 1;
    sop.sem_num = 0;
    sop.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sop, 1) < 0) {
        perror("semop");
        return -1;
    }
    sop.sem_op = -1;
    if (semop(sem_id, &sop, 1) < 0) {
        perror("semop");
        return -1;
    }
    time_t count = time(NULL);
    struct tm* tm = localtime(&count);
    printf("%d:%d:%d running...\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
    // you can start another program here
    sleep(10);
    count = time(NULL);
    tm = localtime(&count);
    printf("%d:%d:%d end...\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
    sop.sem_op = 1;
    if (semop(sem_id, &sop, 1) < 0) {
        perror("semop");
        return -1;
    }
    sleep(1);
    semctl(sem_id, 0, IPC_RMID);
}
