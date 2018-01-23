#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAXLINE 4096

int main(int argc, char* argv[])
{
    //父进程用来执行add程序，从子进程获取输入参数，并向子进程输出结果
    //子进程则向父进程输入参数，并从父进程获取执行结果
    int fd1[2]; // child read,parent write
    int fd2[2]; // child write,parent read
    pipe(fd1);
    pipe(fd2);
    pid_t pid;
    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid == 0) {
        close(fd1[1]);
        close(fd2[0]);
        char line[MAXLINE];
        while (fgets(line, MAXLINE, stdin)) {
            write(fd2[1], line, strlen(line));
            int n = read(fd1[0], line, MAXLINE);
            line[n] = 0;
            fputs(line, stdout);
        }
    } else {
        close(fd1[0]);
        close(fd2[1]);
        dup2(fd1[1], STDOUT_FILENO);
        dup2(fd2[0], STDIN_FILENO);
        execl("./08_add2", "07_add1", NULL);
        int status;
        waitpid(pid, &status, 0);
        printf("parent done\n");
    }
    return 0;
}
