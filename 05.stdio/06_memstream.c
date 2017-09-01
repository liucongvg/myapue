#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BSZ 48

int main(int argc, char* argv[])
{
    FILE* fp;
    char buf[BSZ];
    memset(buf, 'a', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    if ((fp = fmemopen(buf, BSZ, "w+")) == NULL) {
        perror("fmemopen error");
        return -1;
    }
    printf("initial buffer content:%s\n", buf); // truncated
    fprintf(fp, "hello, world");
    printf("before flush:%s\n", buf);
    fflush(fp); // null bytes appended
    printf("after flush:%s\n", buf);
    printf("len: %ld\n", strlen(buf));

    memset(buf, 'b', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    fprintf(fp, "hello, world");
    fseek(fp, 0, SEEK_SET); // seek will cause flush, which will cause null bytes appended
    printf("after seek: %s\n", buf);
    printf("len: %ld\n", strlen(buf));

    memset(buf, 'c', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    fprintf(fp, "hello, world");
    fclose(fp); // no null bytes appended
    printf("after close: %s\n", buf);
    printf("len: %ld\n", strlen(buf));
    return 0;
}
