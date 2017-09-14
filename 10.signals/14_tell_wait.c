#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static volatile sig_atomic_t signal_flag;
static void sig_usr1(int signo);
sigset_t new_mask, old_mask, empty_mask;

int tell_wait()
{
    sigemptyset(&new_mask);
    sigemptyset(&empty_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigaddset(&new_mask, SIGUSR2);
    struct sigaction sia_usr1;
    sia_usr1.sa_handler = sig_usr1;
    if (sigaction(SIGUSR1, &sia_usr1, NULL) < 0)
        return -1;
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0)
        return -1;
    return 0;
}

int wait_parent()
{
    while (!signal_flag)
        sigsuspend(&empty_mask);
    signal_flag = 0;
    return sigprocmask(SIG_SETMASK, &old_mask, NULL);
}

int tell_parent(pid_t pid) { return kill(pid, SIGUSR1); }

int wait_child()
{
    while (!signal_flag)
        sigsuspend(&empty_mask);
    signal_flag = 0;
    return sigprocmask(SIG_SETMASK, &old_mask, NULL);
}

int tell_child(pid_t pid) { kill(pid, SIGUSR1); }

static void sig_usr1(int signo) { signal_flag = 1; }

#define PARENT_FIRST 0

int main(int argc, char* argv[])
{
    pid_t pid;
    tell_wait();
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
#if PARENT_FIRST
        printf("in child, start wait for parent\n");
        wait_parent();
        printf("child wait parent done\n");
#else
        printf("in child, start sleep for 3 seconds\n");
        sleep(3);
        printf("child sleep done, tell parent\n");
        tell_parent(getppid());
#endif
    } else {
#if PARENT_FIRST
        printf("in parent, start sleep for 3 seconds\n");
        sleep(3);
        printf("parent sleep done, tell child\n");
        tell_child(pid);
#else
        printf("in parent, start wait for child\n");
        wait_child();
        printf("parent wait child done\n");
#endif
    }
    return 0;
}
