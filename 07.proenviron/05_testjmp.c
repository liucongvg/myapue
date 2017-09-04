#include <setjmp.h>
#include <stdio.h>

static void prin_jmp(int auto_var, int register_var, int volatile_var, int static_var);
static int global_var;
static jmp_buf jmpbuffer;

int main(int argc, char* argv[])
{
    int auto_var;
    register int register_var;
    volatile int volatile_var;
    static int static_var;

    global_var = 1;
    auto_var = 2;
    register_var = 3;
    volatile_var = 4;
    static_var = 5;
    if (setjmp(jmpbuffer) != 0) {
        printf("after longjmp:\n");
        printf("global_var:%d,auto_var:%d,register_var:%d,volatile_var:%d,static_var:%d\n", global_var, auto_var,
            register_var, volatile_var, static_var);
        return 0;
    }
    global_var = 95;
    auto_var = 96;
    register_var = 97;
    volatile_var = 98;
    static_var = 99;
    prin_jmp(auto_var, register_var, volatile_var, static_var);
}

static void prin_jmp(int auto_var, int register_var, int volatile_var, int static_var)
{
    printf("in prin_jmp:\n");
    printf("global_var:%d,auto_var:%d,register_var:%d,volatile_var:%d,static_var:%d\n", global_var, auto_var,
        register_var, volatile_var, static_var);
    longjmp(jmpbuffer, 1);
}
