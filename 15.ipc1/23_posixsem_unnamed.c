#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    void* sem = mmap(NULL, sizeof(sem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sem == (void*)-1) {
        perror("mmap");
        return -1;
    }
    if (sem_init(sem, 1, 1) < 0) {
        perror("sem_init");
        return -1;
    }
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        sleep(1); // let parent run first
        if (sem_wait(sem) < 0) {
            perror("sem_wait");
            return -1;
        }
        time_t current_time = time(NULL);
        struct tm* current_tm = localtime(&current_time);
        printf("%d:%d:%d run...\n", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);
        sem_post(sem);
    } else {
        if (sem_wait(sem) < 0) {
            perror("sem_wait");
            return -1;
        }
        time_t current_time = time(NULL);
        struct tm* current_tm = localtime(&current_time);
        printf("%d:%d:%d start...\n", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);
        sleep(5);
        current_time = time(NULL);
        current_tm = localtime(&current_time);
        printf("%d:%d:%d end...\n", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);
        sem_post(sem);
        // sem_close(&sem); sem_close is used for named sem
        int status;
        waitpid(pid, &status, 0);
        sem_destroy(sem);
    }
    return 0;
}
