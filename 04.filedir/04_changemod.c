#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    struct stat statbuf;
    if (stat("foo", &statbuf) < 0) {
        perror("stat error");
        return -1;
    }
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) { // 0666 before
        perror("chmod error");
        return -1;
    }
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) { // 0600 before,0660 after
        perror("chmod error");
        return -1;
    }
    return 0;
}
