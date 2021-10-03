#ifndef _ncmd_
#define _ncmd_

#include <cstdint>
#include <sys/times.h>
#include <time.h>
#include <sstream>
#include <vector>
using namespace std;

struct process{
    pid_t pid;
    int seconds;
    process():pid(0),seconds(0){}
};

void new_suspend(pid_t id);

void new_resume(pid_t id);

void new_wait(pid_t id);

void new_kill(pid_t id);

void jobs();

#endif