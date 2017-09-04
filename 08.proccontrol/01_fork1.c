#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int global_var = 6;
char buf[] = "a write to stdout\n";

int main(int argc, char* argv[])
{
    int var;
    pid_t pid;
    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) {
        perror("write error");
        return -1;
    }
    printf("before fork\n");
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        global_var++;
        var++;
    } else {
        sleep(2);
    }
    printf("pid = %d,global_var = %d,var = %d\n", getpid(), global_var, var);
    return 0;
}
