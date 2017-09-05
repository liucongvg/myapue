#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        if (execl("/home/liucong/linux/myapue/08.proccontrol/interpreter_file", "interpreter_file", "execl_arg1",
                "execl_arg2", NULL)
            < 0) {
            perror("execl error");
            return -1;
        }
    }
    if (waitpid(pid, NULL, 0) < 0) {
        printf("waitpid error\n");
        return -1;
    }
    return 0;
}
