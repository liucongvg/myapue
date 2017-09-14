#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static void sig_usr1(int signo);
static void sig_alarm(int signo);
// Calling async-signal-unsafe function in prin_mask will cause segment fault
// If I change prin_mask from macro to function, segment fault won't happen, so, all
// in all, do not call async-signal-unsafe function in signal handler no matter if it will
// have race condition or not
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

static sigjmp_buf j_buf;
static volatile sig_atomic_t canjump;

int main(int argc, char* argv[])
{
    struct sigaction s_usr1;
    s_usr1.sa_handler = sig_usr1;
    struct sigaction s_alarm;
    s_alarm.sa_handler = sig_alarm;
    if (sigaction(SIGUSR1, &s_usr1, NULL) < 0) {
        perror("sigaction error");
        return -1;
    }
    if (sigaction(SIGALRM, &s_alarm, NULL) < 0) {
        perror("sigaction error");
        return -1;
    }
    if (sigsetjmp(j_buf, 1)) {
        printf("go into sigsetjmp\n");
        prin_mask(SIGALRM);
        prin_mask(SIGUSR1);
        printf("sigsetjmp, exit now\n");
        exit(0);
    }
    canjump = 1;
    while (1) {
        pause();
    }
    return 0;
}

static void sig_usr1(int signo)
{
    if (!canjump) {
        char* message = "can not jump\n";
        write(STDOUT_FILENO, message, strlen(message));
        return;
    }
    alarm(3);
    time_t current_time;
    time(&current_time);
    while (1) {
        if (time(NULL) > current_time + 5)
            break;
    }
    siglongjmp(j_buf, 1);
}

static void sig_alarm(int signo)
{
    prin_mask(SIGALRM);
    prin_mask(SIGUSR1);
}
