#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/liucong_file_test"

int main(int argc, char* argv[])
{
    mkfifo(FIFO_NAME, S_IRWXU | S_IRWXG | S_IRWXO);
    int fd = open(FIFO_NAME, O_WRONLY);
#define MAXLINE 4096
    char line[MAXLINE];
    fgets(line, MAXLINE, stdin);
    write(fd, line, strlen(line));
    close(fd);
    fd = open(FIFO_NAME, O_RDONLY);
    read(fd, line, MAXLINE);
    close(fd);
    fputs(line, stdout);
    return 0;
}
