#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Two parameters needed!\n");
        exit(EXIT_FAILURE);
    }
    DIR* dir = opendir(argv[1]);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    struct dirent* entry = NULL;
    while ((entry = readdir(dir))) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}
