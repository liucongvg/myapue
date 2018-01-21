#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int fd[2];
    pid_t pid;
    if (argc != 2)
        return -1;
    printf("argv[1]:%s\n", argv[1]);
    FILE* stream = fopen(argv[1], "r");
    if (!stream) {
        perror("fopen error");
        return -1;
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
    } else if (pid > 0) {
        close(fd[0]);
#define MAXLINE 4096
        char line[MAXLINE];
        printf("begin to get\n");
        while (fgets(line, MAXLINE, stream)) {
            write(fd[1], line, strlen(line));
            // printf("write:%s", line);
        }
        if (ferror(stream)) {
            perror("fgets error");
        }
        printf("\n");
        close(fd[1]);
        int status;
        waitpid(pid, &status, 0);
        return 0;
    } else {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        execl("/bin/more", "more", NULL);
    }
}
