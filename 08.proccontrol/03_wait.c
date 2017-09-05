#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void pr_exit(int status);

int main(int argc, char* argv[])
{
    pid_t pid;
    int status;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0)
        exit(7);
    if (wait(&status) != pid) {
        printf("wait error\n");
        return -1;
    }
    pr_exit(status);
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0)
        abort();
    if (wait(&status) != pid) {
        printf("wait error\n");
        return -1;
    }
    pr_exit(status);
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0)
        status /= 0;
    if (wait(&status) != pid) {
        printf("wait error");
        return -1;
    }
    pr_exit(status);
    return 0;
}

void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status:%d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf(
            "abnormal termination, signal number:%d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (core dumped)" : "");
    else if (WIFSTOPPED(status))
        printf("child stoped, signal number:%d\n", WSTOPSIG(status));
    else if (WIFCONTINUED(status))
        printf("child continued, signal number:%d\n", SIGCONT);
}
