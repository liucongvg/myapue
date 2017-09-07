#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) { return 0; }

static void sig_alarm1(int signo) {}
/*
 * 1,module invoke this method will get his SIGALRM handler be replaced by sig_alarm1
 * 2,module invoke this method will get his alarm be replaced(only one alarm exists)
 * 3,race condition between alarm(seconds) and pause()
 */
unsigned int sleep1(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alarm1) == SIG_ERR) {
        perror("signal error");
        exit(-1);
    }
    alarm(seconds);
    pause();
    return alarm(0);
}
