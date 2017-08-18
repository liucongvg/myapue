#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXLINE 4096

int main(int argc, char* argv[])
{
    char buf[MAXLINE];
    pid_t pid;
    int status;
    printf("%% "); /*print prompt*/
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = '\0'; /* replace newline with null */
        }
        if ((pid = fork()) < 0) {
            perror("fork error");
        } else if (pid == 0) {
            int res = execlp(buf, buf, (char*)0);
            if (res == -1) {
                perror("execute error");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else {
            if ((pid = waitpid(pid, &status, 0)) < 0) {
                perror("waitpid error");
            }
        }
        printf("%% ");
    }
    return 0;
}
