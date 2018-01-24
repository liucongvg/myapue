#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 100

int main(int argc, char* argv[])
{
    void* addr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        sleep(1);
        printf("%s\n", (char*)addr);
        munmap(addr, SIZE);
    } else {
        strcpy((char*)addr, "I am liucong");
        munmap(addr, SIZE);
        int status;
        waitpid(pid, &status, 0);
    }
    return 0;
}
