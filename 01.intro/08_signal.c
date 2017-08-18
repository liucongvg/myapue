#include <signal.h>
#include <stdio.h>
#include <string.h>

static void sig_int(int);

int main(int argc, char* argv[])
{
    if (signal(SIGINT, sig_int) == SIG_ERR)
        perror("set signal handler error");
    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    return 0;
}

void sig_int(int signo) { printf("interrupt ---> signal number: %d\n", signo); }
