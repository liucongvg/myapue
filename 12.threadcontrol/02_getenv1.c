#include <stdio.h>
#include <string.h>

#define MAXSTRINGSZ 4096

static char envbuffer[MAXSTRINGSZ];

extern char** environ;

char* getenv(const char* name)
{
    int i, len = strlen(name);
    for (i = 0; environ[i] != NULL; ++i) {
        if ((strncmp(name, environ[i], len)) == 0 && environ[i][len] == '=') {
            strncpy(envbuffer, &environ[i][len + 1], MAXSTRINGSZ - 1);
            return envbuffer;
        }
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    printf("HOME:%s\n", getenv("HOME"));
    return 0;
}
