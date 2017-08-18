#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 4096

int main(int argc, char* argv[])
{
    int n;
    char buffer[BUFSIZE];
    while ((n = read(STDIN_FILENO, buffer, BUFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, n) != n) {
            perror("write error");
            return -1;
        }
    }
    if (n < 0) {
        perror("read error");
        return -1;
    }
    return 0;
}
