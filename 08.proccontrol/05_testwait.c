#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static void prin_str(const char* str);
static void wait_parent();
static void tell_child();
#define ARRAY_SIZE 500

int main(int argc, char* argv[])
{
    pid_t pid;
    char buffer[ARRAY_SIZE];
    int i;
    for (i = 0; i < ARRAY_SIZE - 1; ++i) {
        buffer[i] = i % 27 == 26 ? '\n' : ('a' + i % 27);
    }
    buffer[ARRAY_SIZE - 1] = '\0';
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if (pid == 0) {
        wait_parent();
        prin_str(buffer);
        return 0;
    } else {
        prin_str(buffer);
        tell_child();
    }
    if (waitpid(pid, NULL, 0) != pid) {
        printf("waitpid error\n");
        return -1;
    }
}

static void prin_str(const char* str)
{
    const char* ptr;
    int c;
    setbuf(stdout, NULL); // don't forget this line, or you're not gonna see race condition
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);
}

static void wait_parent() {}

static void tell_child() {}
