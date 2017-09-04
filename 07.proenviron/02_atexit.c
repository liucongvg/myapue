#include <stdio.h>
#include <stdlib.h>

static void my_exit1(void);
static void my_exit2(void);

int main(int argc, char* argv[])
{
    if (atexit(my_exit1) != 0) {
        printf("atexit error to register my_exit1\n");
        return -1;
    }
    if (atexit(my_exit2) != 0) {
        printf("atexit error to register my_exit2\n");
        return -1;
    }
    if (atexit(my_exit2) != 0) {
        printf("atexit error to register my_exit2\n");
        return -1;
    }
    printf("main done\n");
    // return 0;
    exit(EXIT_SUCCESS);
}

static void my_exit1(void) { printf("my_exit1 invoked\n"); }

static void my_exit2(void) { printf("my_exit2 invoked\n"); }
