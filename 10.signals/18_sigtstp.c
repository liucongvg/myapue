#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static void sig_tstp(int signo);

int main(int argc, char* argv[])
{
    struct sigaction action;
    action.sa_handler = sig_tstp;
    action.sa_flags = SA_RESTART;
    // you could replace the above two line with:action.sa_handler = SIG_DFL;
    // and you will find SIG_DFL has something to do with SA_RESTART
    if (sigaction(SIGTSTP, &action, NULL) < 0) {
        perror("sigaction error");
        return -1;
    }
    int n;
    char buffer[BUFSIZ];
    while ((n = read(STDIN_FILENO, buffer, BUFSIZ)) > 0) {
        if (write(STDOUT_FILENO, buffer, n) < n) {
            perror("write error");
            return -1;
        }
    }
    if (n < 0) {
        perror("read error");
        return -1;
    }
    return 0;
}

// supposing this is hander for vim
static void sig_tstp(int signo)
{
    // first, remember current interface here [1]
    // for example, move cursor to lower left corner, reset tty mode
    //......
    // then unblock the SIGTSTP because SIGTSTP is blocked in signal handler
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    // set handler to default
    struct sigaction action_dfl, action_tstp;
    action_dfl.sa_handler = SIG_DFL;
    action_dfl.sa_flags = SA_RESTART;
    action_tstp.sa_handler = sig_tstp;
    action_tstp.sa_flags = SA_RESTART;
    const char* message = "error\n";
    if (sigaction(SIGTSTP, &action_dfl, NULL) < 0) {
        write(STDOUT_FILENO, message, strlen(message));
    }
    // send SIGTSTP to myself, and the process would stopped until SIGCONT is delivered
    message = "stop myself\n";
    write(STDOUT_FILENO, message, strlen(message));
    kill(getpid(), SIGTSTP);
    message = "continued\n";
    write(STDOUT_FILENO, message, strlen(message));
    // kill returns, that means SIGCONT is delivered, process is now continued
    // so we set the handler to sig_tstp and restore the interface remembered in [1]
    if (sigaction(SIGTSTP, &action_tstp, NULL) < 0)
        write(STDOUT_FILENO, message, strlen(message));
    // restore the interface status here
    //...
    // for example, reset tty mode, redraw screen
}
