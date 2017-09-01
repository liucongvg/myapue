#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 4096

int main(int argc, char* argv[])
{
    char name[L_tmpnam], line[MAXLINE];
    FILE* fp;
    int fd;
    struct stat statbuf;
    printf("%s\n", tmpnam(NULL));
    tmpnam(name);
    printf("%s\n", name);
    if ((fp = tmpfile()) == NULL) {
        perror("tmpfile error");
        return -1;
    }
    if ((fd = fileno(fp)) < 0) {
        perror("fileno error");
        return -1;
    }
    if (fstat(fd, &statbuf) < 0) {
        perror("fstat error");
        return -1;
    }
    fputs("one line of output\n", fp);
    rewind(fp);
    if (fgets(line, sizeof(line), fp) == NULL) {
        if (ferror(fp)) {
            printf("fgets error\n");
            return -1;
        }
    }
    fputs(line, stdout);
    return 0;
}
