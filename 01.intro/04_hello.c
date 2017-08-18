#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    printf("Hello Word from pid:%ld\n", (long)getpid());
    return 0;
}
