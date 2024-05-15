#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

struct Token {
        string type;
        int line;
        string instance;
        int count;
};

typedef enum {
	ERR01 = -1,      // {char} cannot follow {char}
	ERR02 = -2,      // No Token begins with {char} 
	s1 = 0, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13,
	FIN00 = 1000,    // Comment
	FIN01 = 1001,    // Token Type : T1
	FIN02 = 1002,    // Token Type : T2
	FIN03 = 1003,    // Token Type : T3
	FIN04 = 1004,    // Token Type : EOF
	FINNL = 1010	 // Token Type : NL
} state;

#endif
