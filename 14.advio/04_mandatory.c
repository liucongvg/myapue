#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int lockfile(int fd);
void tell_wait();
void tell_parent(pid_t pid);
void wait_child();
void tell_child();
static sigset_t old_set;
static sigset_t empty_set;
volatile sig_atomic_t flag;

int main(int argc, char* argv[])
{
    int fd = open("tempfile", O_RDWR | O_CREAT | O_NONBLOCK, S_IRWXU | S_IRWXO | S_IRWXO);
    if (fd < 0)
        return -1;
    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0)
        return -1;
    if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        return -1;
    tell_wait();
    int pid;
    if ((pid = fork()) < 0)
        return -1;
    else if (pid == 0) {
        if (lockfile(fd) < 0)
            perror("lockfile filed in child");
        tell_parent(getppid());
        sigsuspend(&empty_set);
        sigprocmask(SIG_SETMASK, &old_set, NULL);
    } else {
        wait_child();
        if (lockfile(fd) < 0) {
            perror("lockfile filed in parent");
        }
        char buf[2];
        if (read(fd, buf, 1) < 0)
            perror("read filed in parent");
        tell_child(pid);
        sigprocmask(SIG_SETMASK, &old_set, NULL);
    }
    return 0;
}

int lockfile(int fd)
{
    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = F_WRLCK;
    return fcntl(fd, F_SETLK, &fl);
}

void usr_handler(int signo) { flag = 1; }
void tell_wait()
{
    sigset_t new_set;
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &new_set, &old_set);
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    sigemptyset(&empty_set);
    action.sa_handler = usr_handler;
    sigaction(SIGUSR1, &action, NULL);
}
void tell_parent(pid_t pid) { kill(pid, SIGUSR1); }
void wait_child()
{
    while (!flag)
        sigsuspend(&empty_set);
    flag = 0;
}
void tell_child(pid_t pid) { kill(pid, SIGUSR1); }
