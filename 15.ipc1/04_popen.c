#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

static pid_t* pid_array = NULL;

FILE* my_popen(const char* command, const char* type)
{
    int fd[2];
    pipe(fd);
    pid_t pid;
    long open_max;
#ifdef OPENMAX
    open_max = OPENMAX;
#else
    open_max = sysconf(_SC_OPEN_MAX);
#endif
    printf("open_max:%ld\n", open_max);
    pid_array = (pid_t*)calloc(sizeof(pid_t), open_max);
    if ((pid = fork()) < 0) {
        return NULL;
    } else if (pid == 0) {
        if (type[0] == 'r') {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
        } else if (type[0] == 'w') {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
        } else {
            return NULL;
        }
        execl("/bin/sh", "sh", "-c", command, NULL);
    } else {
        FILE* stream = NULL;
        if (type[0] == 'r') {
            close(fd[1]);
            stream = fdopen(fd[0], "r");
            pid_array[fd[0]] = pid;
        } else if (type[0] == 'w') {
            close(fd[0]);
            pid_array[fd[1]] = pid;
            stream = fdopen(fd[1], "w");
        }
        return stream;
    }
}

int my_pclose(FILE* stream)
{
    if (!pid_array)
        return -1;
    int fd = fileno(stream);
    pid_t pid;
    if ((pid = pid_array[fd]) == 0)
        return -1;
    int status;
    waitpid(pid, &status, 0);
    return status;
}

int main(int argc, char* argv[])
{
    FILE* stream = my_popen("ls", "r");
    char line[4096];
    while (fgets(line, 4096, stream))
        fputs(line, stdout);
    return 0;
}
