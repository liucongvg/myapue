#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// if file is too large, we don't want to run out of virtual address space of the process
#define MAX_MAPSIZE 1024 * 1024

int main(int argc, char* argv[])
{
    if (argc != 3)
        return -1;
    int fd_in = open(argv[1], O_RDONLY);
    int fd_out = open(argv[2], O_RDWR | O_APPEND | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    struct stat in_stat;
    fstat(fd_in, &in_stat);
    ftruncate(fd_out, in_stat.st_size);
    off_t size_written = 0;
    while (size_written < in_stat.st_size) {
        off_t size_to_map;
        if (in_stat.st_size - size_written > MAX_MAPSIZE)
            size_to_map = MAX_MAPSIZE;
        else
            size_to_map = in_stat.st_size - size_written;
        void *src, *dst;
        if ((src = mmap(NULL, size_to_map, PROT_READ, MAP_SHARED, fd_in, size_written)) == MAP_FAILED) {
            perror("mmap1 failed");
            return -1;
        }
        if ((dst = mmap(NULL, size_to_map, PROT_WRITE, MAP_SHARED, fd_out, size_written)) == MAP_FAILED) {
            perror("mmap2 failed");
            return -1;
        }
        memcpy(dst, src, size_to_map);
        munmap(src, size_to_map); // unmap the virtual address space and the file to reclaim the virtual area space
        munmap(dst, size_to_map);
        size_written += size_to_map;
    }
    return 0;
}
