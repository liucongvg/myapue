#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_quit(int signo);

int main(int argc, char* argv[])
{
    sigset_t new_mask, old_mask, pending_mask;
    if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
        perror("signal error");
        return -1;
    }
    if (sigemptyset(&new_mask) < 0) {
        printf("sigemptyset error\n");
        return -1;
    }
    if (sigaddset(&new_mask, SIGQUIT) < 0) {
        printf("sigemptyset error\n");
        return -1;
    }
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        perror("sigprocmask error");
        return -1;
    }
    printf("SIGQUIT blocked, start sleep\n");
    sleep(5);
    printf("end sleep\n");
    if (sigpending(&pending_mask) < 0) {
        perror("sigpending error");
        return -1;
    }
    if (sigismember(&pending_mask, SIGQUIT) == 1)
        printf("SIGQUIT is a member of pending signals\n");
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        perror("sigprocmask error");
        return -1;
    }
    printf("SIGQUIT unblocked\n");
    int n;
    if ((n = sleep(5)) > 0) {
        printf("unslept seconds:%d\n", n);
    }
    return 0;
}

static void sig_quit(int signo)
{
    printf("sig_quit invoked\n");
    // if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
    //    perror("signal error");
    //    exit(-1);
    //}
}
