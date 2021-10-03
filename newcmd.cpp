#include "newcmd.h"
#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <sys/resource.h>
using namespace std;

vector<process> table;

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

int convert_time(string T){
    int t;

    if (T.size()==5){
        int MIN=10*(T[0]-'0')+(T[1]-'0');
        int SEC=10*(T[3]-'0')+(T[4]-'0');

        t=60*MIN+SEC;
    }

    else if (T.size()==8){
        int H=10*(T[0]-'0')+(T[1]-'0');
        int MIN=10*(T[3]-'0')+(T[4]-'0');
        int SEC=10*(T[6]-'0')+(T[7]-'0');

        t=3600*H+60*MIN+SEC;
    }

    return t;
}

void get_pid(){
    char* buf=(char*)malloc(250);

    int fd[2];    
    if (pipe(fd)<0) perror("pipe!");

    pid_t pid=fork();
    if (pid<0) perror("get_pid");

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

        stringstream S(buf);
        string PID;
        S>>PID;
        pid_t id;
        while (S>>id){
            process temp=process();
            temp.pid=id;
            table.push_back(temp);
        }

        free(buf);
    }

    return;
}

void get_time(){
    char* buf=(char*)malloc(250);

    int fd[2];    
    if (pipe(fd)<0) perror("pipe!");

    pid_t pid=fork();
    if (pid<0) perror("get_time");

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

        stringstream S(buf);
        string elapsed,TIME;
        S>>elapsed;
        int i=0;
        while (S>>TIME){
            table[i].seconds=convert_time(TIME);
            i++;
        }

        free(buf);
    }

    return;
}

void jobs(){
    get_pid();
    get_time();

    cout<<"# of running processes:"<<table.size()<<'\n';
    cout<<"process id's and elapsed time are respectively:\n";
    for (process& p:table) cout<<p.pid<<' '<<p.seconds<<'\n';
    
    rusage usage;
    if (getrusage(RUSAGE_CHILDREN,&usage)<0) perror("getrusage");

    cout<<"Completed Processes:\n";
    cout<<"user time:"<<usage.ru_utime.tv_sec<<'\n';
    cout<<"system time:"<<usage.ru_stime.tv_sec<<'\n';

    return;
}