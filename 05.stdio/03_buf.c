#include <errno.h>
#include <stdio.h>

void print_stdio(const char* name, FILE* fp);
int is_unbuffered(FILE* fp);
int is_linebuffered(FILE* fp);
int buffer_size(FILE* fp);

int main(int argc, char* argv[])
{
    FILE* fp;
    fputs("enter a character\n", stdout);
    if (getchar() == EOF) {
        printf("getchar error\n");
        return -1;
    }
    fputs("one line to standard error\n", stderr);
    print_stdio("stdin", stdin);
    print_stdio("stdout", stdout);
    print_stdio("stderr", stderr);

    if ((fp = fopen("/etc/passwd", "r")) == NULL) {
        perror("fopen error");
        return -1;
    }
    if (getc(fp) == EOF)
        if (ferror(fp)) {
            printf("getc error\n");
            return -1;
        }
    print_stdio("/etc/passwd", fp);
}

void print_stdio(const char* name, FILE* fp)
{
    printf("stream = %s, ", name);
    if (is_unbuffered(fp))
        printf("unbuffered");
    else if (is_linebuffered(fp))
        printf("line buffered");
    else
        printf("fully buffered");
    printf(", buffer size = %d\n", buffer_size(fp));
}

int is_unbuffered(FILE* fp) { return fp->_flags & _IO_UNBUFFERED; }

int is_linebuffered(FILE* fp) { return fp->_flags & _IO_LINE_BUF; }

int buffer_size(FILE* fp)
{
    //#ifdef _LP64
    //    return (fp->_base - fp->_ptr);
    //#else
    return BUFSIZ;
    //#endif
}
