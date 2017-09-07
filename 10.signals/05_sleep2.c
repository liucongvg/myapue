#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static jmp_buf alarm_jmp;

static void sig_alarm2(int signo) { longjmp(alarm_jmp, 1); }
unsigned int sleep2(unsigned int seconds);

/*
 * 1,module invoke sleep2 will get his SIGALRM handler be replaced with sig_alarm2
 * 2,signal mask won't restore
 * 3,sig_alarm2 may be invoked when before setjmp which is a undefined error
 * 4,if a signal handler invoke sleep2, it will exit ahead of time
 * */
unsigned int sleep2(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alarm2) == SIG_ERR) {
        perror("signal error");
        return seconds;
    }
    if (setjmp(alarm_jmp) == 0) {
        alarm(seconds);
        pause();
    }
    return alarm(0);
}

static void sig_int(int signo)
{
    volatile int i, j, k = 0;
    printf("start computing\n");
    for (i = 0; i < 3000000; ++i)
        for (j = 0; j < 4000; ++j)
            k += i * j;
    printf("end computing\n");
}

int main(int argc, char* argv[])
{
    signal(SIGINT, sig_int);
    printf("start sleep\n");
    unsigned int un_sleeped = sleep2(2);
    printf("eng sleep\n");
    printf("un_sleeped seconds:%u\n", un_sleeped);
    return 0;
}
