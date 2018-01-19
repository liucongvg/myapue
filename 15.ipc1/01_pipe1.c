#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

int main(int argc, char* argv[])
{
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        perror("open pipe error");
        return -1;
    }
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork error");
    } else if (pid == 0) {
        close(pipe_fd[1]);
        char line[MAXLINE] = { 0 };
        if (read(pipe_fd[0], line, sizeof(line)) < 0) {
            perror("read pipe error");
        }
        printf("child process read:%s", line);
    } else {
        close(pipe_fd[0]);
        char* message = "hello liucong\n";
        if (write(pipe_fd[1], message, strlen(message)) < 0) {
            perror("write pipe error");
        }
    }
    return 0;
}
