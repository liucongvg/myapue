#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sig_alarm(int signo) {}

unsigned int sleep(unsigned int seconds)
{
    struct sigaction action, old_action;
    action.sa_handler = sig_alarm;
    if (sigaction(SIGALRM, &action, &old_action) < 0)
        return -1;
    sigset_t block_alarm, saved_block_alarm;
    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGALRM);
    if (sigprocmask(SIG_BLOCK, &block_alarm, &saved_block_alarm) < 0)
        return -1;
    alarm(seconds);
    sigset_t current = saved_block_alarm;
    sigdelset(&current, SIGALRM);
    sigsuspend(&current);
    int unsleeped = alarm(0); // what if a signal occured and the signal handler also invoked the alarm function?
    if (sigaction(SIGALRM, &old_action, NULL) < 0)
        return -1;
    return unsleeped;
}

int main(int argc, char* argv[])
{
    printf("start...\n");
    sleep(3);
    printf("end...\n");
    return 0;
}
