#ifndef _cmd_
#define _cmd_

#include <cstdint>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;
typedef vector<string> VS;

void the_sleep(int s);

void any_cmd(int argc,VS& argv,string& in,string& out);

#endif