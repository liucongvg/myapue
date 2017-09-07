#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static void sig_alarm(int signo);

int main(int argc, char* argv[])
{
    struct passwd* pt;
    signal(SIGALRM, sig_alarm);
    alarm(1);
    while (1) {
        errno = 0;
        if ((pt = getpwnam("liucong")) == NULL) {
            if (errno != 0) {
                perror("getpwnam error");
                return -1;
            } else {
                printf("null entry\n");
                return -1;
            }
        }
        if (strcmp("liucong", pt->pw_name) != 0)
            printf("return value corrupted!, pw_name = %s\n", pt->pw_name);
    }
}

static void sig_alarm(int signo)
{
    printf("SIGALRM recieved start\n");
    struct passwd* pt;
    if ((pt = getpwnam("root")) == NULL) {
        if (errno != 0) {
            perror("getpwnam error");
            exit(-1);
        } else {
            printf("null entry\n");
            exit(-1);
        }
    }
    alarm(1);
    printf("SIGALRM recieved end\n");
}
