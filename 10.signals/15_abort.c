#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void abort(void)
{
    printf("abort start...\n");
    struct sigaction action;
    if (sigaction(SIGABRT, NULL, &action) < 0) {
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }
    if (action.sa_handler = SIG_IGN) {
        action.sa_handler = SIG_DFL;
        if (sigaction(SIGABRT, &action, NULL) < 0) {
            perror("sigaction error");
            exit(EXIT_FAILURE);
        }
    }
    if (action.sa_handler = SIG_DFL)
        fflush(NULL);
    sigset_t set_abort;
    sigfillset(&set_abort);
    sigdelset(&set_abort, SIGABRT);
    if (sigprocmask(SIG_SETMASK, &set_abort, NULL) < 0) {
        perror("sigprocmask error");
        exit(EXIT_FAILURE);
    }
    kill(getpid(), SIGABRT);
    // caller has its own handler to caught SIGABRT is we can get here
    fflush(NULL);
    action.sa_handler = SIG_DFL;
    if (sigaction(SIGABRT, &action, NULL) < 0) {
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }
    // sigprocmask agin in case of caller abort handler change the proc mask
    if (sigprocmask(SIG_SETMASK, &set_abort, NULL) < 0) {
        perror("sigprocmask error");
        exit(EXIT_FAILURE);
    }
    kill(getpid(), SIGABRT);
    // should never get here
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    abort();
    return 0;
}
