#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int system(const char* cmd);
void pr_exit(int status);

int main(int argc, char* argv[]) // the status return from system is shell not executable
{
    if (argc != 2) {
        printf("need one argument as system command\n");
        return -1;
    }
    int status;
    if ((status = system("date")) < 0) {
        printf("date error\n");
        return -1;
    }
    pr_exit(status);
    if ((status = system("nosuchcommand")) < 0) {
        printf("nosuchcommand error\n");
        return -1;
    }
    pr_exit(status);
    if ((status = system("who;exit 44")) < 0) {
        printf("who;exit 44 error\n");
        return -1;
    }
    pr_exit(status);
    if ((status = system(argv[1])) < 0) {
        printf("%s error\n", argv[1]);
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

int system(const char* cmd)
{
    pid_t pid;
    int status;
    if (cmd == NULL)
        return 1;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        _exit(127);
    } else {
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }
    return status;
}
