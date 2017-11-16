#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void lock_a_byte(const char* name, int fd, int offset);
static void tell_wait();
static void wait_parent();
static void wait_child();
static void tell_parent();
static void tell_child();

int main(int argc, char* argv[])
{
    int fd = creat("tempfile", S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0)
        return -1;
    write(fd, "aa", 2);
    tell_wait();
    pid_t pid;
    if ((pid = fork()) < 0)
        return -1;
    else if (pid == 0) {
        lock_a_byte("child", fd, 0);
        tell_parent(getppid());
        wait_parent();
        lock_a_byte("child", fd, 1);
    } else {
        lock_a_byte("parent", fd, 1);
        tell_child(pid);
        wait_child();
        sleep(1);
        lock_a_byte("parent", fd, 0);
    }
    return 0;
}

static void lock_a_byte(const char* name, int fd, int offset)
{
    struct flock my_lock;
    my_lock.l_len = 1;
    my_lock.l_whence = SEEK_SET;
    my_lock.l_start = offset;
    my_lock.l_type = F_WRLCK;
    int res = fcntl(fd, F_SETLK, &my_lock);
    if (res < 0) {
        fprintf(stderr, "%s lock:%d ", name, offset);
        perror("fcntl error");
    } else
        printf("%s hold lock, offset:%d\n", name, offset);
}
static volatile sig_atomic_t flag;
static void sig_usr1(int sig_nu)
{
    flag = 1;
    printf("sig_usr1\n");
}
static struct sigaction action;
static sigset_t old_set;
static void tell_wait()
{
    sigset_t new_set;
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &new_set, &old_set);
    action.sa_handler = sig_usr1;
    sigemptyset(&action.sa_mask);
    sigaction(SIGUSR1, &action, NULL);
}
static void wait()
{
    sigset_t set;
    sigemptyset(&set);
    while (!flag)
        sigsuspend(&set);
    sigprocmask(SIG_SETMASK, &old_set, NULL);
}
static void wait_parent()
{
    printf("wait parent\n");
    wait();
}
static void wait_child()
{
    printf("wait child\n");
    wait();
}
static void tell(pid_t pid) { kill(pid, SIGUSR1); }
static void tell_parent(pid_t pid)
{
    printf("tell parent\n");
    tell(pid);
}
static void tell_child(pid_t pid)
{
    printf("tell child\n");
    tell(pid);
}
