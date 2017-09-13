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
// should not use signal unsafe method in signal handler, remember!
void prin_mask(int signo, const char* str)
{
    sigset_t current_mask;
    if (sigprocmask(SIG_BLOCK, NULL, &current_mask) < 0) {
        perror("sigprocmask error");
        exit(-1);
    }
    int res;
    if ((res = sigismember(&current_mask, signo)) < 0) {
        perror("sigismember error");
        exit(-1);
    } else if (res == 1) {
        printf("%s is a member of current proc mask\n", str);
    } else {
        printf("%s is not a member of current proc mask\n", str);
    }
}
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
        prin_mask(SIGALRM, "SIGALRM");
        prin_mask(SIGUSR1, "SIGUSR1");
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
    prin_mask(SIGALRM, "SIGALRM");
    prin_mask(SIGUSR1, "SIGUSR1");
}
