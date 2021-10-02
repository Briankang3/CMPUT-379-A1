#ifndef _ncmd_
#define _ncmd_

#include <cstdint>
#include <sys/times.h>
#include <time.h>
#include <vector>
using namespace std;

struct process{
    pid_t id;
    int seconds;
    process(pid_t i,int t):id(i),seconds(t){}
};

void new_suspend(pid_t id);

void new_resume(pid_t id);

void new_wait(pid_t id);

void new_kill(pid_t id);

void jobs(tms* d);

#endif