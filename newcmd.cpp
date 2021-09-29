#include "newcmd.h"
#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <iostream>
#include <sstream>
using namespace std;

void new_suspend(pid_t id){
    // suspend the process with pid id
    if (kill(id,SIGSTOP)==-1) perror("new_suspend");

    return;
}

void new_resume(pid_t id){
    // resume the process with id
    if (kill(id,SIGCONT)==-1) perror("new_resume");

    return;
}

void new_wait(pid_t id){
    // wait for the process to finish
    pid_t p=fork();
    if (p<0) perror("new_wait");
    
    else if (p==0){
        if (waitpid(id,NULL,WNOHANG)==-1) perror("new_wait");
    }

    return;
}

void new_kill(pid_t id){
    // terminate the process
    pid_t p=fork();
    if (p<0) perror("new_kill");

    else if (p==0){
        if (kill(id,SIGKILL)==-1) perror("new_kill");
    }

    return;
}

void jobs(){
    char buf[100];
    int fd[2];

    pid_t pid=fork();
    if (pid<0) perror("new_ps");
    
    if (pipe(fd)<0) perror("pipe!");

    if (pid==0){
        // the child process runs "ps" and pipes it into the parent process
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);

        if (execl("/usr/bin/ps","ps",nullptr)<0) perror("unable to run ps");
    }

    else{
        wait(NULL);
        // extract information
        close(fd[1]);
        string i;
        cin>>i;
        cout<<i;
        close(fd[0]);
    }
}