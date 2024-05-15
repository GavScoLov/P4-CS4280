#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstring>

#include "token.h"

using namespace std;

// File Structure Values
struct File {
	char* file;
	char* bFile;
	string ttlInst;
	int ln;	
};
// Setup Node Structure
struct Node {
	int type;
	Token t1;
	Token t2;
	Token t3;
	Token t4;
	Node* ch1;
	Node* ch2;
	Node* ch3;
	Node* ch4;
};
// Store Node and File Info for passing back to previous node
struct NFInfo {
	Node* node;
	File info;
	Token tk;
	int count;
	string identifier;
	string id[100];
	int idCount;
	int tokenCount;
};

#endif
