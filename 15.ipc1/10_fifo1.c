#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/liucong_file_test"

//从program2中读取字符串，然后在开头加上from program1:并写回给program2
int main(int argc, char* argv[])
{
    mkfifo(FIFO_NAME, S_IRWXU | S_IRWXG | S_IRWXO);
    int fd = open(FIFO_NAME, O_RDONLY);
#define MAXLINE 4096
    char line[MAXLINE];
    read(fd, line, MAXLINE);
    close(fd);
    fd = open(FIFO_NAME, O_WRONLY);
    char new_line[MAXLINE];
    sprintf(new_line, "from program1:%s", line);
    write(fd, new_line, strlen(new_line));
    close(fd);
    return 0;
}
