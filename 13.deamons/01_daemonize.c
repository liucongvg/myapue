#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

void daemonize(const char* cmd)
{
    umask(0);

    pid_t pid;
    if ((pid = fork()) < 0)
        return;
    else if (pid != 0)
        exit(0);

    setsid();

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    sigaction(SIGHUP, &action, NULL);

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
    pause();
    return;
}

int main(int argc, char* argv[])
{
    daemonize("test");
    return 0;
}
