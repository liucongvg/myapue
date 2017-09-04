#include <setjmp.h>
#include <stdio.h>

#define MAXLINE 4096
jmp_buf jmpbuffer;

static void method1();
static void method2();

int main(int argc, char* argv[])
{
    char line[MAXLINE];
    if (setjmp(jmpbuffer) != 0)
        printf("error occur\n");
    while (fgets(line, MAXLINE, stdin) != NULL) {
        fputs(line, stdout);
        method1();
    }
    return 0;
}

static void method1()
{
    printf("method1 invoked\n");
    method2();
    printf("reture from method1\n");
}

static void method2()
{
    printf("method2 invoked\n");
    longjmp(jmpbuffer, 1);
    printf("return from method2\n");
}
