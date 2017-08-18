#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
        perror("lseek error");
    } else {
        printf("lseek successfully\n");
    }
    return 0;
}
