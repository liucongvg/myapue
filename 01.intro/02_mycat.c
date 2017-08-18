#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 4096

int main(int argc, char* argv[])
{
    int n;
    char buffer[BUFFSIZE];
    while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0) {
        int res = write(STDOUT_FILENO, buffer, n);
        if (res == -1)
            fprintf(stderr, "error write:%s\n", strerror(errno));
    }
    if (n == -1)
        fprintf(stderr, "error read:%s\n", strerror(errno));
    return 0;
}
