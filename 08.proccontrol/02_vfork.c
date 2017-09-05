#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int global_var = 6;

int main(int argc, char* argv[])
{
    int var = 88;
    pid_t pid;
    printf("before vfork\n");
    if ((pid = vfork()) < 0) {
        perror("vfork error");
        return -1;
    } else if (pid == 0) {
        global_var++;
        var++;
        printf("child pid:%d\n", getpid());
        _exit(0);
    }
    printf("parent pid = %u,global_var = %d,var = %d\n", getpid(), global_var, var);
    return 0;
}
