#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void sig_hup(int signo);
static void pr_ids(const char* name);

int main(int argc, char* argv[])
{
    char c;
    pid_t pid;
    pr_ids("parent");
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid > 0) {
        sleep(2);
    } else {
        pr_ids("child");
        struct sigaction siga;
        siga.sa_handler = sig_hup;
        sigaction(SIGHUP, &siga, NULL);
        kill(getpid(), SIGTSTP);
        pr_ids("child");
        if (read(STDIN_FILENO, &c, 1) != 1)
            perror("read error on controlling TTY");
    }
    return 0;
}

static void sig_hup(int signo) { printf("SIGHUP recieved, pid:%d\n", getpid()); }

static void pr_ids(const char* name)
{
    printf("%s:pid:%d,ppid:%d,pgrp:%d,tpgrp:%d\n", name, getpid(), getppid(), getpgrp(), tcgetpgrp(STDIN_FILENO));
}
