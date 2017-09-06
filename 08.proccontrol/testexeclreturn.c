#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char* argv)
{
    printf("this is testexeclreturn\n");
    abort();
    printf("after abort\n");
}
