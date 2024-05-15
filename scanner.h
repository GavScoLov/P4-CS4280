#ifndef TESTSCANNER_H
#define TESTSCANNER_H

#include <iostream>
#include <string>

#include "node.h"

using namespace std;

//const int MAX = 100;

string filter(char*);

string fsaDriver(char*, char*, string, string);

Token scanner(char*, char*, string, int);

#endif

