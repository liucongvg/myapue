#include <stdio.h>

#define MAXLINE 4096

int main(int argc, char* argv[])
{
    char buf[MAXLINE];
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF) {
            printf("fputs error\n");
            return -1;
        }
    }
    if (ferror(stdin)) {
        printf("fputs error\n");
        return -1;
    }
    return 0;
}
