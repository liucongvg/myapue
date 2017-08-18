#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    umask(0);                                                                          // 0
    if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) { // 0666
        perror("create error");
        return -1;
    }
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);                                      // 0066
    if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) { // 0666
        perror("create error");
        return -1;
    }
    return 0;
}
