#ifndef _ncmd_
#define _ncmd_

#include <cstdint>
#include <sys/times.h>
#include <time.h>
using namespace std;

void new_suspend(pid_t id);

void new_resume(pid_t id);

void new_wait(pid_t id);

void new_kill(pid_t id);

void jobs();

#endif