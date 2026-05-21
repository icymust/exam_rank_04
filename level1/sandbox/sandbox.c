#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

void alarm_handler(int sig)
{
    (void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose){

    struct sigaction sa;
    int status;
    pid_t pid;

    sa.sa_handler = alarm_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, 0);

    pid = fork();

    if(pid <0)
        return(-1);

    if(pid == 0){
        f();
        exit(0);
    }

    alarm(timeout);

    if(waitpid(pid, &status, 0) == -1){
        if(errno == EINTR){
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            if(verbose)
                printf("Bad function: timed out after %u seconds\n", timeout);
            return(0);
        }
        return(-1);
    }
    alarm(0);
    if(WIFEXITED(status)){
        if(WEXITSTATUS(status) == 0){
            if(verbose)
                printf("Nice function!\n");
            return(1);
        }
        else{
            if(verbose)
                printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
            return(0);
        }
    }

    if(WIFSIGNALED(status)){
        if(verbose)
            printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
        return(0);
    }
    return(-1);
}

// void good(void){
//     while(1){}
//     exit(1);
// }

// int main(){
//     sandbox(good, 2, true);
//     return(0);
// }