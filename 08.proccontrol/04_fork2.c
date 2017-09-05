#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) { // first child
        if ((pid = fork()) < 0) {
            perror("fork error");
            return -1;
        } else if (pid > 0) {
            printf("make %d exit\n", getpid());
            exit(0); // make parent exit
        }
        // do something
        sleep(2); // wait for parent to exit so that I can make init be my father
        printf("second child, parent pid = %d\n", getppid());
        return 0;
    }
    printf("waitpid for:%d\n", pid);
    if (waitpid(pid, NULL, 0) != pid) {
        printf("waitpid error\n");
        return -1;
    }
    printf("waitpid done, mypid:%d\n", getpid());
    return 0;
}
