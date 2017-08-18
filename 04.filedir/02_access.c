#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("one parameter required!");
        return -1;
    }
    if (access(argv[1], R_OK) < 0) {
        perror("access error");
        return -1;
    }
    if (open(argv[1], O_RDONLY) < 0) {
        perror("open error");
        return -1;
    } else {
        printf("open for reading OK\n");
    }
    return 0;
}
