#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static char buffer[500000];

int main(int argc, char* argv[])
{
    int n_to_write = read(STDIN_FILENO, buffer, sizeof(buffer));
    int flags = fcntl(STDOUT_FILENO, F_GETFL);
    fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK);
    char* temp = buffer;
    while (n_to_write > 0) {
        int n_writen = write(STDOUT_FILENO, temp, n_to_write);
        if (n_writen > 0) {
            n_to_write -= n_writen;
            temp += n_writen;
        }
        fprintf(stderr, "n_writen:%d,errno:%d\n", n_writen, errno);
    }
    flags = fcntl(STDOUT_FILENO, F_GETFL);
    fcntl(STDOUT_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    return 0;
}
