#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
#define MAXLINE 4096
    char line[MAXLINE];
    while (fgets(line, MAXLINE, stdin)) {
        int a, b;
        sscanf(line, "%d%d", &a, &b);
        sprintf(line, "%d\n", a + b);
        fputs(line, stdout);
        fflush(stdout);
    }
}
