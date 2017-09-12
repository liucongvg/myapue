#include <errno.h>
#include <signal.h>
#include <stdio.h>

#define SIGBAD(signo) ((signo <= 0) || (signo) >= NSIG)
#define sigemptyset(ptr) (*(ptr) = 0)
#define sigfillset(ptr) (*(ptr) = ~(sigset_t)0, 0)
/**
 * this code won't compile through, sigset_t is not implemented as integer type but structure,
 * and bit operations can only be used for integer types.
 **/

/**int sigaddset(sigset_t* set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }
    *set |= 1 << (signo - 1);
    return 0;
}

int sigdelset(sigset_t* set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }
    *set &= ~(1 << (signo - 1));
    return 0;
}

int sigismember(const sigset_t* set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }
    return (*set & (1 << (signo - 1))) != 0;
}**/

int main(int argc, char* argv[]) { return 0; }
