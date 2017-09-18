#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int system(const char* command)
{
    struct sigaction saved_sigint, saved_sigquit, ignored;
    ignored.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &ignored, &saved_sigint) < 0) {
        perror("sigaction error");
        return -1;
    }
    if (sigaction(SIGQUIT, &ignored, &saved_sigquit) < 0) {
        perror("sigaction error");
        return -1;
    }
    sigset_t sigint_block, saved_sigint_block;
    sigemptyset(&sigint_block);
    if (sigaddset(&sigint_block, SIGCHLD) < 0) {
        return -1;
    }
    if (sigprocmask(SIG_BLOCK, &sigint_block, &saved_sigint_block) < 0) {
        perror("sigprocmask error");
        return -1;
    }
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        if (sigaction(SIGINT, &saved_sigint, NULL) < 0) {
            perror("sigaction error");
            exit(-1);
        }
        if (sigaction(SIGQUIT, &saved_sigquit, NULL) < 0) {
            perror("sigaction error");
            exit(-1);
        }
        if (sigprocmask(SIG_SETMASK, &saved_sigint_block, NULL) < 0) {
            perror("sigprocmask error");
            exit(-1);
        }
        if (execl("/bin/sh", "sh", "-c", command, NULL) < 0) {
            perror("execl error");
            exit(-1);
        }
        _exit(127);
    }
    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR) {
            status = -1;
            break;
        }
    }
    if (sigaction(SIGINT, &saved_sigint, NULL) < 0) {
        perror("sigaction error");
        exit(-1);
    }
    if (sigaction(SIGQUIT, &saved_sigquit, NULL) < 0) {
        perror("sigaction error");
        exit(-1);
    }
    if (sigprocmask(SIG_SETMASK, &saved_sigint_block, NULL) < 0) {
        perror("sigprocmask error");
        return -1;
    }
    return status;
}

void sig_child(int signo)
{
    const char* message = "sig_child\n";
    write(STDOUT_FILENO, message, strlen(message));
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, 0)) < 0) {
        if (errno == ECHILD)
            message = "ECHILD\n";
        write(STDOUT_FILENO, message, strlen(message));
        if (errno != EINTR)
            break;
    }
    if (pid == 0)
        message = "pid == 0\n";
    else
        message = "pid != 0\n";
    write(STDOUT_FILENO, message, strlen(message));
}

int main(int argc, char* argv[])
{
    struct sigaction action;
    action.sa_handler = sig_child;
    if (sigaction(SIGCHLD, &action, NULL) < 0) {
        perror("sigaction error");
        return -1;
    }
    if (system("ls") < 0) {
        perror("error");
        return -1;
    }
}
