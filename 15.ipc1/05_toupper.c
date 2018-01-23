#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int in = getchar();
    while ((in = getchar()) != EOF) {
        if (isalpha(in) && islower(in)) {
            putchar(toupper(in));
        } else {
            putchar(in);
            if (in == '\n')
                fflush(stdout);
        }
    }
    return 0;
}
