#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void make_temp(char* template);

int main(int argc, char* argv[])
{
    char right_template[] = "/tmp/fileXXXXXX";
    char* wrong_template = "/tmp/fileXXXXXX";
    printf("creating first temp file\n");
    make_temp(right_template);
    printf("creating second temp file\n");
    make_temp(wrong_template);
    return 0;
}

void make_temp(char* template)
{
    int fd;
    struct stat statbuf;
    if ((fd = mkstemp(template)) < 0) {
        perror("mkstemp error");
        return;
    }
    printf("temp file name:%s\n", template);
    close(fd);
    if (stat(template, &statbuf) < 0) {
        perror("stat error");
        return;
    } else {
        printf("file exists\n");
        unlink(template);
    }
}
