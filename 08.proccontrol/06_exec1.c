#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char* env_init[] = { "USER=liucong", "liucong=666", NULL };

int main(int argc, char* argv[])
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        if (execle("/home/liucong/linux/myapue/08.proccontrol/07_echoall", "07_echoall", "07_echoall", "myarg1",
                "myarg2", NULL, env_init)
            < 0) {
            perror("execle error");
            return -1;
        }
    }
    if (waitpid(pid, NULL, 0) < 0) {
        printf("waitpid error\n");
        return -1;
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        if (execlp("07_echoall", "07_echoall", "arg1", NULL) < 0) {
            perror("execlp error");
            return -1;
        }
    }
    if (waitpid(pid, NULL, 0) < 0) {
        printf("waitpid error\n");
        return -1;
    }
    return 0;
}
