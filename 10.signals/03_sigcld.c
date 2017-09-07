#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

static void sig_child(int signo);

int main(int argc, char* argv[]) // Linux has no this kind of mess
{
    pid_t pid;
    if (signal(SIGCLD, sig_child) == SIG_ERR) {
        perror("signal error");
        return -1;
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        sleep(2);
        exit(1);
    } else {
        pause();
        return 0;
    }
}

static void sig_child(int signo)
{
    printf("enter into sig_child\n");
    signal(SIGCLD, sig_child);
    pid_t pid;
    if ((pid = waitpid(-1, NULL, 0)) < 0) {
        printf("waitpid error\n");
    } else {
        printf("child:%d\n", pid);
    }
    printf("sig_child return\n");
}
