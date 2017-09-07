#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sig_usr(int signo);

int main(int argc, char* argv[])
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        perror("signal error");
        return -1;
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        perror("signal error");
        return -1;
    }
    while (1) {
        printf("start pause\n");
        pause();
        printf("end pause\n");
    }
}

static void sig_usr(int signo)
{
    if (signo == SIGUSR1)
        printf("SIGUSR1 recieved\n");
    if (signo == SIGUSR2)
        printf("SIGUSR2 recieved\n");
}
