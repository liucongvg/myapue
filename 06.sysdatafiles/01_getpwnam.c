#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

struct passwd* getpwnam(const char* name);

int main(int argc, char* argv[])
{
    struct passwd* pt = getpwnam("liucong");
    printf("gecos:%s,dir:%s,passwd:%s,name:%s,shell:%s\n", pt->pw_gecos, pt->pw_dir, pt->pw_passwd, pt->pw_name,
        pt->pw_shell);
    return 0;
}

struct passwd* getpwnam(const char* name)
{
    struct passwd* ptr;
    setpwent();
    errno = 0;
    while ((ptr = getpwent()) != NULL) {
        if (!strcmp(name, ptr->pw_name))
            break;
    }
    endpwent();
    if (errno != 0) {
        perror("getpwnam error");
        return NULL;
    }
    return ptr;
}
