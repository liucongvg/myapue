#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void sig_int(int signo);

#define prin_mask(signo)                                                                                               \
    do {                                                                                                               \
        sigset_t current_mask;                                                                                         \
        if (sigprocmask(SIG_BLOCK, NULL, &current_mask) < 0) {                                                         \
            const char* error_message = "sigprocmask error\n";                                                         \
            write(STDOUT_FILENO, error_message, strlen(error_message));                                                \
            exit(-1);                                                                                                  \
        }                                                                                                              \
        int res;                                                                                                       \
        if ((res = sigismember(&current_mask, signo)) < 0) {                                                           \
            const char* error_message = "sigismember error\n";                                                         \
            write(STDOUT_FILENO, error_message, strlen(error_message));                                                \
            exit(-1);                                                                                                  \
        } else if (res == 1) {                                                                                         \
            const char* prin_message = #signo " is a member of current proc mask\n";                                   \
            write(STDOUT_FILENO, prin_message, strlen(prin_message));                                                  \
        } else {                                                                                                       \
            const char* prin_message = #signo " is a not member of current proc mask\n";                               \
            write(STDOUT_FILENO, prin_message, strlen(prin_message));                                                  \
        }                                                                                                              \
    } while (0)

/*
 * correct way to protect a critical region of code from a specific
signal*/
int main(int argc, char* argv[])
{
    sigset_t new_mask, old_mask, suspend_mask;
    sigemptyset(&new_mask);
    sigemptyset(&suspend_mask);
    sigaddset(&new_mask, SIGINT);
    sigaddset(&suspend_mask, SIGQUIT);
    struct sigaction siga;
    siga.sa_handler = sig_int;
    if (sigaction(SIGINT, &siga, NULL) < 0) {
        perror("sigaction error");
        return -1;
    }
    printf("before sigprocmask:\n");
    prin_mask(SIGINT);
    prin_mask(SIGQUIT);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    printf("after sigprocmask SIGINT:\n");
    prin_mask(SIGINT);
    prin_mask(SIGQUIT);
    printf("before sigsuspend:\n");
    if (sigsuspend(&suspend_mask) < 0) {
        perror("sigsuspend error");
        // return -1;
    }
    printf("after sigsuspend:\n");
    prin_mask(SIGINT);
    prin_mask(SIGQUIT);
    printf("before sigprocmask unblock SIGINT\n");
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        perror("sigprocmask error");
        return -1;
    }
    printf("after sigprocmask unblock SIGINT:\n");
    prin_mask(SIGINT);
    prin_mask(SIGQUIT);
    printf("pausing...\n");
    pause();
    return 0;
}

static void sig_int(int signo)
{
    const char* message = "in sig_int\n";
    write(STDOUT_FILENO, message, strlen(message));
    prin_mask(SIGINT);
    prin_mask(SIGQUIT);
}
