#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#define LOCK_FILE "/home/liucong/01_daemonize@liucong"
#define LOCK_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
static sigset_t mask;

int already_running(void)
{
    int fd = open(LOCK_FILE, O_RDWR | O_CREAT, LOCK_MODE);
    if (fd < 0)
        return -1;
    if (lockf(fd, F_TLOCK, 0) < 0) {
        close(fd);
        exit(-1);
    }
    ftruncate(fd, 0);
    char buffer[16] = { 0 };
    sprintf(buffer, "%d", getpid());
    write(fd, buffer, strlen(buffer));
    return 0;
}

void reread() {}

void* thread_function(void* argv)
{
    while (1) {
        int signal;
        sigwait(&mask, &signal);
        switch (signal) {
        case SIGHUP:
            reread();
            break;
        case SIGINT:
            exit(1);
        case SIGTERM:
            exit(1);
        default:
            break;
        }
    }
}

void daemonize(const char* cmd)
{
    umask(0);

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    sigaction(SIGHUP, &action, NULL);

    pid_t pid;
    if ((pid = fork()) < 0)
        return;
    else if (pid != 0)
        exit(0);

    setsid();

    chdir("/");

    struct rlimit fd_limit;
    getrlimit(RLIMIT_NOFILE, &fd_limit);
    if (fd_limit.rlim_max == RLIM_INFINITY)
        fd_limit.rlim_max = 1024;
    unsigned long i;
    for (i = 0; i < fd_limit.rlim_max; ++i)
        close(i);
    int fd0, fd1, fd2;
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "fd error");
        exit(-1);
    }
    if (already_running() < 0)
        exit(-1);
    action.sa_handler = SIG_DFL;
    sigaction(SIGHUP, &action, NULL);
    sigfillset(&mask);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_function, NULL);
    pause();
    return;
}

int main(int argc, char* argv[])
{
    daemonize("test");
    return 0;
}
