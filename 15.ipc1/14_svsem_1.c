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
    union semun {
        int val;               /* Value for SETVAL */
        struct semid_ds* buf;  /* Buffer for IPC_STAT, IPC_SET */
        unsigned short* array; /* Array for GETALL, SETALL */
        struct seminfo* __buf; /* Buffer for IPC_INFO
                                  (Linux-specific) */
    };
    union semun to_set;
    struct semid_ds sds;
    semctl(sem_id, 0, IPC_STAT, to_set);
    printf("to_set.buf->sem_nsems:%ld\n", to_set.buf->sem_nsems);
    struct sembuf sop;
    sop.sem_op = -1;
    sop.sem_num = 0;
    sop.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sop, 1) < 0) {
        perror("semop");
        return -1;
    }
    struct tm* tm = localtime(NULL);

    printf("%d:%d:%d running...\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
    sleep(5);
    printf("%d:%d:%d end...\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
    sop.sem_op = 1;
    if (semop(sem_id, &sop, 1) < 0) {
        perror("semop");
        return -1;
    }
    sleep(1);
    semctl(sem_id, 0, IPC_RMID);
}
