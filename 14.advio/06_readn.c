#include <unistd.h>

ssize_t readn(int fd, void* ptr, size_t n)
{
    size_t n_left = n;
    ssize_t n_read;
    while (n_left > 0) {
        if ((n_read = read(fd, ptr, n_left)) < 0) {
            if (n_left == n)
                return -1;
            else
                break;
        } else if (n_read == 0)
            break;
        n_left -= n_read;
        ptr += n_read;
    }
    return (n - n_left);
}

ssize_t writen(int fd, void* ptr, size_t n)
{
    size_t n_left = n;
    ssize_t n_written;
    while (n_left > 0) {
        if ((n_written = write(fd, ptr, n_left)) < 0) {
            if (n_left == n)
                return -1;
            else
                break;
        } else if (n_written == 0)
            break;
        n_left -= n_written;
        ptr += n_written;
    }
    return (n - n_left);
}

int main(int argc, char* argv[]) { return 0; }
