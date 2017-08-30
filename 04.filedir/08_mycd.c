#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (chdir("/tmp") < 0) {
        perror("chdir error");
        return -1;
    }
    printf("successfully chdir to /tmp\n");
    return 0;
}
