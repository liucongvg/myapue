#include <stdio.h>

int main(int argc, char* argv[])
{
    int c;
    while ((c = getc(stdin)) != EOF) {
        if (putc(c, stdout) == EOF) {
            printf("putc error\n");
            return -1;
        }
    }
    if (ferror(stdin)) {
        printf("getc error\n");
        return -1;
    }
    return 0;
}
