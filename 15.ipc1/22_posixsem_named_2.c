#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define PATHNAME "/liucong_posixsem_named"

int main(int argc, char* argv[])
{
    sem_t* sem = sem_open(PATHNAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }
    if (sem_wait(sem) < 0) {
        perror("sem_wait");
        return -1;
    }
    time_t current_time = time(NULL);
    struct tm* current_tm = localtime(&current_time);
    printf("%d:%d:%d run...\n", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);
    sem_post(sem);
    sem_close(sem);
    return 0;
}
