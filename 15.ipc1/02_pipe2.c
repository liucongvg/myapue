#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
        return -1;
    int fd[2];
    pipe(fd);
    pid_t pid;
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
            // printf("write:%s", line);
            write(fd[1], line, strlen(line));
        }
        if (ferror(stream)) {
            perror("fgets error");
        }
        close(fd[1]);
        int status;
        waitpid(pid, &status, 0);
        return 0;
    } else {
        close(fd[1]);
        if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
            perror("dup2 error");
        }
        /*
         * 此处more是如何实现的，直接在终端中敲more，more并没有阻塞，而是输出了一段东西
         * 这就说明more不是通过读标准输入来阻塞自己，一旦有数据进来就处理并显示的，那么
         * more是如何实现的呢？为什么这里有数据重定向到more的标准输入它就会自动按page输出呢？
         */
        if (execl("/bin/more", "more", NULL) < 0) {
            perror("execl error");
        }
    }
}
