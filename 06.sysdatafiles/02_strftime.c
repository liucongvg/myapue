#include <errno.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[])
{
    time_t t;
    struct tm* tmp;
    char buf1[16];
    char buf2[64];
    if (time(&t) < 0) {
        perror("time error");
        return -1;
    }
    tmp = localtime(&t);
    if (strftime(buf1, sizeof(buf1), "time and date: %r, %a %b %d, %Y", tmp) == 0)
        printf("buffer length %ld is too small\n", sizeof(buf1));
    else
        printf("%s\n", buf1);
    if (strftime(buf2, sizeof(buf2), "time and date: %r, %a %b %d, %Y", tmp) == 0)
        printf("buffer length %ld is too small\n", sizeof(buf2));
    else
        printf("%s\n", buf2);
    return 0;
}
