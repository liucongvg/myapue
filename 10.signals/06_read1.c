#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 4096
static void sig_alarm(int signo) { printf("sig_alarm invoked\n"); }

int main(int argc, char* argv[])
{
    if (signal(SIGALRM, sig_alarm) == SIG_ERR) {
        perror("signal error");
        return -1;
    }
    char buffer[MAXLINE];
    int count = 0;
    alarm(4);
    if ((count = read(STDIN_FILENO, buffer, MAXLINE)) < 0) { // when signal handlers are installed with the signal
                                                             // function, interrupted system calls will be restarted
        perror("read error");
        return -1;
    }
    write(STDOUT_FILENO, buffer, count);
    alarm(0);
    return 0;
}
