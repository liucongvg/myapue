#include <stdio.h>

int main(int argc, char* argv[])
{
    FILE* stream = popen("./05_toupper", "r");
#define MAXLINE 4096
    char line[MAXLINE];
    while (fgets(line, MAXLINE, stream)) {
        fputs(line, stdout);
        fflush(stdout);
    }
    fclose(stream);
    return 0;
}
