#include <errno.h>
#include <stdio.h>
#include <signal.h>

static void sig_quit(int signo);

int main(int argc,char *argv[]){
    sigset_t new_mask,old_mask,pending_mask;
    if(sigemptyset(&new_mask)<0){
        printf("sigemptyset error\n");
        return -1;
    }
    if(sigaddset(&new_mask,SIGQUIT)<0){
        printf("sigemptyset error\n");
        return -1;
    }
    if(sigprocmask(SIG_BLOCK,&new_mask,&old_mask)<0){
        perror("sigprocmask error");
        return -1;
    }

}
