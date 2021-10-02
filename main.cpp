#include "newcmd.h"
#include "cmd.h"
#include <string>
#include <iostream>
#include <sys/wait.h>
#include <sstream>
using namespace std;

int main(){
    tms d,t;
    clock_t D=times(&d);
    clock_t C;

    while (1){
        string input;
        cout<<"SHELL379:"<<endl;
        cin>>input;

        if (input.compare("exit")==0){
            // wait until all processes finish
            while (wait(nullptr)>0);
            // print out the time
            C=times(&t);
            cout<<"Resources Used:\n";
            cout<<"User time = ";
            cout<<t.tms_utime-d.tms_utime<<'\n';
            cout<<"System time = ";
            cout<<t.tms_stime-d.tms_stime<<'\n';

            // exit the entire program
            exit(0);
        }

        if (input.compare("kill")==0){
            pid_t id;
            cin>>id;
            new_kill(id);
        }

        else if (input.compare("sleep")==0){
            int s;
            cin>>s;
            the_sleep(s);
        }

        else if (input.compare("suspend")==0){
            pid_t id;
            cin>>id;
            new_suspend(id);
        }

        else if (input.compare("wait")==0){
            pid_t id;
            cin>>id;
            new_wait(id);
        }

        else if (input.compare("resume")==0){
            pid_t id;
            cin>>id;
            new_resume(id);
        }

        else if (input.compare("jobs")==0) jobs(&d);
        
        else{
            VS argv;
            argv.push_back(input);
            int argc=1;
            string in,out;

            string i;
            getline(cin,i);
            stringstream S(i);

            string s;
            while (S>>s){
                if (s[0]=='<') in=s.substr(1,s.size()-1);

                else if (s[0]=='>') out=s.substr(1,s.size()-1);
                
                else{
                    argv.push_back(s);
                    argc++;
                }
            }

            any_cmd(argc,argv,in,out);
        }
    }

    return 0;
}