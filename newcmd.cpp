#include "newcmd.h"
#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <bits/stdc++.h>
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

void get_pid(char buf[250]){
    int fd[2];
    pid_t pid=fork();
    if (pid<0) perror("get_pid");
    
    if (pipe(fd)<0) perror("pipe!");

    if (pid==0){
        // the child process runs "ps" and pipes it into the parent process
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);

        if (execl("/usr/bin/ps","ps","-o","pid",nullptr)<0) perror("unable to get pids");
    }

    else{
        wait(NULL);
        close(fd[1]);

        int n=read(fd[0],buf,250);
        if (n<0) perror("unable to read in get_pid");
        close(fd[0]);
    }

    return;
}

void get_time(char buf[250]){
    int fd[2];
    pid_t pid=fork();
    if (pid<0) perror("get_time");
    
    if (pipe(fd)<0) perror("pipe!");

    if (pid==0){
        // the child process runs "ps" and pipes it into the parent process
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);

        if (execl("/usr/bin/ps","ps","-o","etime",nullptr)<0) perror("unable to get time");
    }

    else{
        wait(NULL);
        close(fd[1]);

        int n=read(fd[0],buf,250);
        if (n<0) perror("unable to read in get_time");
        
        close(fd[0]);
    }

    return;
}

void jobs(tms* d){
    vector<process> table;

    char* buf_one;
    char* buf_two;
    buf_one=(char*)malloc(250);
    buf_two=(char*)malloc(250);

    get_pid(buf_one);
    get_time(buf_two);

    cout<<string(buf_one)<<'\n';
    cout<<string(buf_two)<<'\n';
    
    tms t;
    clock_t C;

    C=times(&t);
    cout<<"Resources Used:\n";
    cout<<"User time = ";
    cout<<t.tms_utime-d->tms_utime<<'\n';
    cout<<"System time = ";
    cout<<t.tms_stime-d->tms_stime<<'\n';

    free(buf_one);
    free(buf_two);

    return;
}