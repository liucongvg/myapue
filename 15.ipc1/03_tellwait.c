#include <stdio.h>
#include <unistd.h>

int fd[2];
void tell_wait() { pipe(fd); }

void wait()
{
    close(fd[1]);
    char c[1];
    read(fd[0], c, 1);
}
void tell()
{
    close(fd[0]);
    write(fd[1], "c", 1);
}

void wait_parent() { wait(); }
void tell_child() { tell(); }

void wait_child() { wait(); }
void tell_parent() { tell(); }

int main(int argc, char* argv[])
{
    pid_t pid;
    tell_wait();
    printf("let child run first\n");
    if ((pid = fork()) < 0) {
        perror("fork error");
    } else if (pid > 0) {
        wait_child();
        printf("parent run...\n");
    } else {
        printf("child run start...\n");
        sleep(3);
        printf("child run end...\n");
        tell_parent();
    }
    return 0;
}
