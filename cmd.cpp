#include "cmd.h"
#include <cerrno>
#include <string>
#include <sys/wait.h>
#include <iostream>
#include <fcntl.h>
#define MAX_LENGTH 20
using namespace std;

void the_sleep(int s){
    pid_t id=fork();
    if (id<0) perror("the_sleep");

    else if (id==0){
        const char* i=to_string(s).c_str();
        if (execl("/usr/bin/sleep","sleep",i,NULL)==-1) perror("the_sleep");
    }

    wait(NULL);
    return;
}

void any_cmd(int argc,VS& argv,string& in,string& out){
    const char *cmd=argv[0].c_str();
    pid_t pid=fork();
    if (pid<0) perror("any_cmd");

    else if (pid==0){
        // child process
        // convert the input into char**
        if (argv.back().compare("&")==0){
            argv.pop_back();
            argc--;
        }

        const char *args[argc+1];
        for (int i=0;i<argc;i++) args[i]=argv[i].c_str();
        args[argc]=nullptr;

        if (!in.empty()){
            // input redirection
            // pipe it into this process
            int ffd=open(in.c_str(),O_CREAT|O_RDONLY,0644);
            if (ffd<0) perror("cannot open the input redirection");

            dup2(ffd,STDIN_FILENO);
            close(ffd);
        }

        if (!out.empty()){
            // output redirection
            int ffd=open(out.c_str(),O_CREAT|O_WRONLY);
            if (ffd<0) perror("cannot open the output redirection");

            dup2(ffd,STDOUT_FILENO);
            close(ffd);
        }

        if (execvp(cmd,const_cast<char **>(args))<0) perror("error executing the command");
    }

    return;
}