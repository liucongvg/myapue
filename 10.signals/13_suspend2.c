#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static volatile sig_atomic_t quit_flag;
static void sig_int_quit(int signo);

int main(int argc, char* argv[])
{
    sigset_t new_mask, old_mask, zero_mask;
    sigemptyset(&new_mask);
    sigemptyset(&zero_mask);
    sigaddset(&new_mask, SIGINT);
    struct sigaction sig;
    sig.sa_handler = sig_int_quit;
    if (sigaction(SIGINT, &sig, NULL) < 0) {
        perror("sigaction error");
        return -1;
    }
    if (sigaction(SIGQUIT, &sig, NULL) < 0) {
        perror("sigaction error");
        return -1;
    }
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        perror("sigprocmask error");
        return -1;
    }
    while (!quit_flag)
        sigsuspend(&zero_mask);
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        perror("sigprocmask error");
        return -1;
    }
    return 0;
}

static void sig_int_quit(int signo)
{
    if (signo == SIGINT) {
        const char* message = "SIGINT recieved\n";
        write(STDOUT_FILENO, message, strlen(message));
    } else if (signo == SIGQUIT) {
        const char* message = "SIGQUIT recieved\n";
        write(STDOUT_FILENO, message, strlen(message));
        quit_flag = 1;
    }
}
