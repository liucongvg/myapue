#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int a, b;
#define MAXLINE 4096
    char line[MAXLINE];
    while (read(STDIN_FILENO, line, MAXLINE) > 0) {
        sscanf(line, "%d%d", &a, &b);
        sprintf(line, "%d\n", a + b);
        write(STDOUT_FILENO, line, strlen(line));
    }
    return 0;
}
