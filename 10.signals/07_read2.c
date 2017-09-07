#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_alarm(int signo);
#define MAXLINE 4096
static jmp_buf jmp_alarm;

int main(int argc, char* argv[])
{
    if (signal(SIGALRM, sig_alarm) == SIG_ERR) {
        perror("signal error");
        return -1;
    }
    int count;
    char buffer[MAXLINE];
    if (setjmp(jmp_alarm) != 0) {
        printf("read interrupted, return\n");
        return 0;
    }
    alarm(3);
    if ((count = read(STDIN_FILENO, buffer, MAXLINE)) < 0) {
        perror("read error");
        return -1;
    } else {
        write(STDOUT_FILENO, buffer, count);
    }
    alarm(0);
    return 0;
}

static void sig_alarm(int signo) { longjmp(jmp_alarm, 1); }
