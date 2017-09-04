#include <errno.h>
#include <stdio.h>

int main(int argc, char* argv[]) { return 0; }

FILE* open_data(void)
{
    FILE* fp;
    char databuf[BUFSIZ];
    if ((fp = fopen("datafile", "r")) == NULL) {
        perror("fopen error");
        return NULL;
    }
    if (setvbuf(fp, databuf, _IOLBF, BUFSIZ) != 0) {
        printf("setvbuf error\n");
        return NULL;
    }
    return fp;
}
