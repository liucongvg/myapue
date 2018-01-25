#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHMANME "/liucong_posixshm"
#define FILE_SIZE 100

int main(int argc, char* argv[])
{
    int fd = shm_open(SHMANME, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        return -1;
    }
    void* addr = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    snprintf(addr, FILE_SIZE, "I am liucong\n");
    close(fd);
    munmap(addr, FILE_SIZE);
}
